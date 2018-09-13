import cvxopt

import numpy as np


def lp_general_graph(unaries, edges, edge_weights):
    n_nodes, n_states = list(map(int, unaries.shape))
    n_edges = len(edges)

    n_variables = n_nodes * n_states + n_edges * n_states ** 2

    n_constraints = n_nodes + n_edges * (2 * n_states - 1)

    edges_offset = n_nodes * n_states
    data, I, J = [], [], []

    # summation constraints
    for i in range(n_nodes):
        for j in range(n_states):
            data.append(1)
            I.append(i)
            J.append(i * n_states + j)
    row_idx = n_nodes
    for i in range(2 * n_edges * n_states):
        edge = i // (2 * n_states)
        state = (i % n_states)
        vertex_in_edge = i % (2 * n_states) // n_states
        vertex = edges[edge][vertex_in_edge]
        if vertex_in_edge == 1 and state == n_states - 1:
            continue
        data.append(-1)
        I.append(row_idx)
        J.append(int(vertex) * n_states + state)
        edge_var_index = edges_offset + edge * n_states ** 2
        if vertex_in_edge == 0:
            for j in range(n_states):
                data.append(1)
                I.append(row_idx)
                J.append(edge_var_index + state * n_states + j)
        else:
            for j in range(n_states):
                data.append(1)
                I.append(row_idx)
                J.append(edge_var_index + j * n_states + state)
        row_idx += 1

    coef = np.ravel(unaries)
    repeated_pairwise = edge_weights.ravel()
    coef = np.hstack([coef, repeated_pairwise])
    c = cvxopt.matrix(coef, tc='d')
    G = cvxopt.spdiag(cvxopt.matrix(-np.ones(n_variables)))
    h = cvxopt.matrix(np.zeros(n_variables))  # for positivity inequalities
    A = cvxopt.spmatrix(data, I, J)
    b_ = np.zeros(A.size[0])
    b_[:n_nodes] = 1
    b = cvxopt.matrix(b_)

    show_progress_backup = cvxopt.solvers.options.get('show_progress', False)
    cvxopt.solvers.options['show_progress'] = False
    result = cvxopt.solvers.lp(c, G, h, A, b)
    cvxopt.solvers.options['show_progress'] = show_progress_backup

    x = np.array(result['x'])
    unary_variables = x[:n_nodes * n_states].reshape(n_nodes, n_states)
    pairwise_variables = x[n_nodes * n_states:].reshape(n_edges, n_states ** 2)
    return unary_variables, pairwise_variables, result['primal objective']


def _validate(unary, pairwise, edges):
    n_states = unary.shape[-1]
    if pairwise.shape == (n_states, n_states):
        # only one matrix given
        pairwise_potentials = np.repeat(pairwise[np.newaxis, :, :],
                                        edges.shape[0], axis=0)
    else:
        if pairwise.shape != (edges.shape[0], n_states, n_states):
            raise ValueError()
        pairwise_potentials = pairwise
    return n_states, pairwise_potentials


def my_inference_dispatch(unary_potentials, pairwise_potentials, edges):
    """
    Works slow but for small inputs it's okay.
    """
    relaxed = False
    return_energy = False
    shape_org = unary_potentials.shape[:-1]
    n_states, pairwise_potentials = \
        _validate(unary_potentials, pairwise_potentials, edges)

    unaries = unary_potentials.reshape(-1, n_states)
    res = lp_general_graph(-unaries, edges, -pairwise_potentials)
    unary_marginals, pairwise_marginals, energy = res
    if relaxed:
        unary_marginals = unary_marginals.reshape(unary_potentials.shape)
        y = (unary_marginals, pairwise_marginals)
    else:
        y = np.argmax(unary_marginals, axis=-1)
        y = y.reshape(shape_org)
    if return_energy:
        return y, energy
    return y

import Data.Char

-- 1 -------------------------------
id_ :: a -> a
id_ x = x

eval :: (a -> b, a) -> b
eval (f, x) = f x

exchange :: (a, b) -> (b, a)
exchange (x, y) = (y, x)

compose :: (b -> c) -> (a -> b) -> a -> c
compose f g = f . g
-- или без оператора (.):
-- compose f g x = f $ g x

curry_ :: ((a,b) -> c) -> (a -> b -> c)
curry_ f x y = f (x, y)

associate :: (a, (b, c)) -> ((a, b), c)
associate (x, (y, z)) = ((x, y), z)

-- 2 -------------------------------
minMax :: Ord a => [a] -> Maybe (a, a) -- (min, max)
minMax [] = Nothing
minMax (x:xs) = case tail_minmax of Just(minimum, maximum) -> Just(min x minimum, max x maximum)
                                    Nothing -> Just(x, x)
        where tail_minmax = minMax xs

-- 3 -------------------------------
-- Работают с отрицательными числами
digitsCount :: Integer -> Integer
digitsCount n
    | abs n < 10 = 1
    | otherwise  = 1 + digitsCount (n `quot` 10)

digitsSum :: Integer -> Integer
digitsSum 0 = 0
digitsSum n = r + digitsSum q
    where (q, r) = divMod (abs n) 10

-- 4 -------------------------------
findMajority :: Eq a => [a] -> Maybe(a)
findMajority [] = Nothing
findMajority (x:xs) = Just(helper (x, 1) xs)
    where   helper (z, _) [] = z
            helper (z, 0) (y:ys) = helper (y, 1) ys
            helper (z, n) (y:ys)
                | z == y = helper (z, n + 1) ys
                | z /= y = helper (z, n - 1) ys

-- 5 -------------------------------
-- Немного расширил область определения, чтобы использовать f в 6 задаче.
f :: (a -> a) -> Int -> (a -> a)
f g n
    | n < 0     = error "n must be non-negative number"
    | n == 0    = id
    | otherwise = g . (f g (n - 1))

-- 6 -------------------------------
-- Остатки чисел фибоначчи цикличны по модулю. Для модуля 10 длина цикла равна 60. 
-- Значит fib(n) mod 10 = fib(n mod 60) mod 10. Работает за O(1). Работает с отрицательными индексами.
fib10 :: Int -> Int
fib10 n = fst $ f (\(f0, f1) -> (f1, (f0 + f1) `mod` 10)) ((abs n) `mod` 60) (0, 1)

-- 7 -------------------------------
isPalindrome :: String -> Bool
isPalindrome s = s_low == reverse s_low
    where s_low = map toLower s

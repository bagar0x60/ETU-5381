import Data.Char
import Data.List
import Data.Foldable

-- 1 -------------------------------
circShiftL :: Int -> [a] -> [a]
circShiftL _ [] = []
circShiftL n xs = right ++ left
    where   shift = n `mod` (length xs)
            (left, right) = splitAt shift xs

-- 2 -------------------------------     
indices :: [a] -> [(Integer, a)]
indices = zip [0..]

zeroBy :: Monoid a => [a] -> (a -> Bool) -> [a]
zeroBy xs p = map (\x -> if p x then x else mempty) xs

triplewiseSum :: [Integer] -> [Integer] -> [Integer] -> [Integer]
triplewiseSum a b c = map (\(x, y, z) -> x + y + z) $ zip3 a b c

-- 3 -------------------------------  
revRange :: (Char,Char) -> [Char]
revRange (a, b) = unfoldr fun b
    where fun x
            | x >= a    = Just (x, chr $ pred $ ord x)
            | otherwise = Nothing

-- 4 -------------------------------  
seriesK :: Int -> [Rational]
seriesK k = (toRational 1) : (map (\x -> x / (toRational k)) (seriesK k))

-- 5 -------------------------------  
newtype SortedList a = SortedList { getSorted :: [a] } deriving (Eq, Show)

instance Ord a => Semigroup (SortedList a) where 
    (SortedList xs) <> (SortedList ys) = SortedList (merge xs ys)
        where   merge [] ys = ys
                merge xs [] = xs
                merge (x:xs) (y:ys) = if x > y 
                                        then y : merge (x:xs) ys
                                        else x : merge xs (y:ys)

instance Ord a => Monoid (SortedList a) where 
    mempty = SortedList []

-- 6 ------------------------------- 
msort :: Ord a => [a] -> SortedList a
msort [] = mempty
msort [x] = SortedList [x]
msort xs = msort left <> msort right
    where  (left, right) = splitAt ((length xs) `div` 2) xs

-- 7 -------------------------------

data Tree a = Nil | Node (Tree a) a (Tree a) deriving (Eq, Show)
newtype Preorder a      = PreO (Tree a)     deriving (Eq, Show)
newtype Postorder a     = PostO (Tree a)    deriving (Eq, Show)
newtype Levelorder a    = LevelO (Tree a)   deriving (Eq, Show)

instance Foldable Tree where
    foldMap f Nil = mempty
    foldMap f (Node l m r) = (foldMap f l) <> f m <> (foldMap f r)
    
instance Foldable Preorder where
    foldMap f (PreO Nil) = mempty
    foldMap f (PreO (Node l m r)) = f m <> (foldMap f (PreO l)) <> (foldMap f (PreO r))

instance Foldable Postorder where
    foldMap f (PostO Nil) = mempty
    foldMap f (PostO (Node l m r)) = (foldMap f (PostO l)) <> (foldMap f (PostO r)) <> f m

instance Foldable Levelorder where
    foldMap f (LevelO t)  = foldl1 (<>) (map (f . getValue) nodesInLevelOrder) 
        where   nodesInLevelOrder = concat (unfoldr nextLevel [t])
                
                nextLevel nodes = if (null nodes) 
                    then Nothing 
                    else Just (nodes, concatMap getChilds nodes)
                
                getValue (Node _ m _) = m
                getChilds (Node l _ r) =  nonNil [l, r]               
                
                nonNil = filter (\x -> case x of 
                                            Nil -> False
                                            _ -> True)


-- tree = Node (Node (Node Nil [4] Nil) [2] (Node Nil [5] Nil)) [1] (Node Nil [3] Nil)
-- foldMap id (LevelO tree)
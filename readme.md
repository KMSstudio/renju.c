# renju.c  
**Kang Myeong Seok. | CSE 2024-10387**

---

renju.c is gomoku engine degisn for midterm project in programming experience lecture.  
renju.c add four new feature from gomoku.c  

### feature1: catch 3x3 rule
If player 1, the black stone tries to make 3 by 3, program will cancle the action.  

    A B C D E F G H I J K L M N O
    1 . . . . . . . . . . . . . . . 
    2 . . 1 . . . . . . . . . . . .
    3 . 1 1 . . . . . . . . . 2 2 . 
    4 . . . . . . . . . . . . 2 . .
    5 . . . . . . . . . . . . . . .
    6 . . . . 1 . . . . . . . 2 . . 
    7 . . . . . . . . . . . . . . .
    8 . . . . . . . . . . . . . . .
    9 . . . . . . . . . . . . . . . 
    10 . . . . . . . . . . . . . . .
    11 . . . . . . . . . . . . . . .
    12 . . . . . . . . . . . . . . .
    13 . . . . . . . . . . . . . . . 
    14 . . . . . . . . . . . . . . .
    15 . . . . . . . . . . . . . . .
    Player 1's turn (enter column and row, e.g., A13): C4
    Invalid move: Rule of 3x3.
    A B C D E F G H I J K L M N O
    1 . . . . . . . . . . . . . . . 
    2 . . 1 . . . . . . . . . . . .
    3 . 1 1 . . . . . . . . . 2 2 . 
    4 . . . . . . . . . . . . 2 . .
    5 . . . . . . . . . . . . . . .
    6 . . . . 1 . . . . . . . 2 . . 
    7 . . . . . . . . . . . . . . .
    8 . . . . . . . . . . . . . . .
    9 . . . . . . . . . . . . . . . 
    10 . . . . . . . . . . . . . . .
    11 . . . . . . . . . . . . . . .
    12 . . . . . . . . . . . . . . . 
    13 . . . . . . . . . . . . . . .
    14 . . . . . . . . . . . . . . . 
    15 . . . . . . . . . . . . . . .
    Player 1's turn (enter column and row, e.g., A13):

## feature2: catch 4x4 rule
Like feature 1, if player 1 tries to make 4 by 4, program will cancle the action

## feature3: catch 6++ rule
if player 1, the balck stone try to make stone 6 or more in a row, program will cancle the action.

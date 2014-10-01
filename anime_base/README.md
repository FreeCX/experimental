Anime Base
============

Программа для оффлайн редактирования списка просмотренного аниме.

##### HOW-TO use regex:
    +         -- progress +1
    -         -- progress -1
    d         -- delete elements {found by title}
    f         -- find by title
    i         -- print this info
    n{new}    -- set name to {new}
    p{number} -- set progress to {number}
    s{number} -- set score to {number}
    s{letter} -- set status { c -- complete, d -- drop, p -- plan, w -- watch }
    w         -- write to database

##### HOW-TO use program:
`$ ./list <database> "f/D.Gray-man/+/-/s7/p23/sc/n/d.gray-man/w"`

    >>  found: '             D.Gray-man', status:    watch, progress:  77 / 103, score: 8 / 10, id: 0
    >> change: '             d.gray-man', status: complete, progress: 103 / 103, score: 7 / 10, id: 0
    >> change saved!

##### Text menu:
Главное текстовое меню в разработке. Поэтому используйте regex.

`$ ./list <database>`

    > menu:
     1 -- input regexp
     2 -- print database
     3 -- print by id
     4 -- find by name
     5 -- write database
     0 -- exit
    > select:

Anime Base
============

Программа для оффлайн редактирования списка просмотренного аниме.

##### HOW-TO use regexp:
    f         -- find by title
    d         -- delete elements {found by title}
    i         -- print this info
    +         -- progress +1
    -         -- progress -1
    s{number} -- set score to {number}
    p{number} -- set progress to {number}
    s{letter} -- set status { c -- complete, d -- drop, p -- plan, w -- watch }
    n{new}    -- set name to {new}
    w         -- write to database

##### HOW-TO use program:
`$ ./list "f/D.Gray-man/+/-/s7/p23/sc/n/d.gray-man/w"`

\>> found elements:
 [id: 0] title: '             D.Gray-man', status:    watch, progress:  77 / 103, score: 8 / 10

\>> changed to:
 [id: 0] title: '             d.gray-man', status: complete, progress: 103 / 103, score: 7 / 10

##### Text menu:
Главное текстовое меню в разработке. Поэтому используйте regexp.

`$ ./list`

    > menu:
     1 -- input regexp
     2 -- print database
     3 -- print by id
     4 -- find by name
     5 -- write database
     0 -- exit
    > select:

Anime Base (alpha version)
============

Программа для оффлайн редактирования списка просмотренного аниме. Используйте на свой страх и риск.

##### Доступные регулярные выражения:
    +        -- номер серии +1
    -        -- номер серии -1
    a        -- добавить элемент
    d        -- удалить элементы {найденые элементы параметром f}
    f        -- поиск по названию
    i        -- распечатать эту информацию
    n        -- изменить имя на новое
    p{число} -- установить номер серии на {число}
    m{число} -- установить максимальный номер серии {0 в случае онгоинга}
    s{число} -- установить рейтинг {число}
    s{буква} -- установить статут { c -- complete, d -- drop, p -- plan, w -- watch }
    w        -- записать изменения в базу

##### Использование программы:
Редактирование существующей записи:
`$ ./list <database> "f/D.Gray-man/+/-/s7/p23/m500/sc/n/d.gray-man/w"`

    >>  found: '             D.Gray-man', status:    watch, progress:  77 / 103, score:  8 / 10, id: 0
    >> change: '             d.gray-man', status: complete, progress: 500 / 500, score:  7 / 10, id: 0
    >> change saved!

Добавление новой записи:
`$ ./list <database> "a/Super Movie 4/p1/m100/s7/sp/w"`

    >> append: '          Super Movie 4', status:     plan, progress:   0 /   0, score:  0 / 10, id: 3
    >> change: '          Super Movie 4', status:     plan, progress:   1 / 100, score:  7 / 10, id: 3
    >> change saved!

Удаление нескольких записей:
`$ ./list <database> "f/D.Gr/f/Code:/d/w"`

    >>  found: '             D.Gray-man', status:    watch, progress:  77 / 103, score:  8 / 10, id: 0
    >>  found: '          Code: Breaker', status: complete, progress:  13 /  13, score:  7 / 10, id: 1
    >> delete: '             D.Gray-man', status:    watch, progress:  77 / 103, score:  8 / 10, id: 0
    >> delete: '          Code: Breaker', status: complete, progress:  13 /  13, score:  7 / 10, id: 1
    >> change saved!

##### Текстовое меню:
Главное текстовое меню в разработке. Поэтому используйте регулярные выражения для редактирования.

`$ ./list <database>`

    > menu:
     1 -- ввести regex
     2 -- вывести базу
     3 -- найти по id
     4 -- найти по имени
     5 -- записать изменения
     0 -- выход
     > select:
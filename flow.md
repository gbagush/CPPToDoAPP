# TODO LIST PROJECT

Kelompok-4 TIB-23

## Index
- <input type="checkbox" checked> New ToDo
- <input type="checkbox" checked> Show ToDos
    - <input type="checkbox" checked> All
    - <input type="checkbox" checked> Today
    - <input type="checkbox" checked> Tommorow
    - <input type="checkbox" checked> Completed
    - <input type="checkbox" checked> Overdue
    - <input type="checkbox" checked> By Category
- <input type="checkbox" checked> Edit ToDo
- <input type="checkbox" checked> Delete ToDo
- <input type="checkbox" checked> Manage Categories
    - <input type="checkbox" checked> New Category
    - <input type="checkbox" checked> Show Category
    - <input type="checkbox" checked> Edit Category
    - <input type="checkbox" checked> Delete Category

## Information

```
[ ] Not finished yet
[V] Finished
[X] Overdue 
```

## Main Menu

```
TODO LIST
1. New ToDo
2. Show ToDos
3. Edit ToDo
4. Delete ToDo
5. Manage Categories
0. Exit
Choice: 
```

## New ToDo

```
NEW TODO
Title:
Description:

Select a category: 
1. School
2. Home
3. Work
Choice:

Deadline (YYYY-MM-DD hh:mm):
```

```
Preview:
[ ] 2023-12-10 23:59 | School
    Linear Algebra (generated id)
    Create a program for calculating linear transformations

Are you sure? (Y/n): Y
```

```
Todo Created Successfully!

1. Create another one
2. Back to menu
Choice: 
```

## Show ToDos

```
SHOW TODOS

1. All todos
2. Today
3. Tommorow
4. Completed
5. Overdue
6. By Category
Choice: 
```

### All ToDos

```
ALL TODOS

Today:
[V] 2023-11-30 23:59 | School
    Linear Algebra (1)
    Create a program for calculating linear transformations

[ ] 2023-11-30 23:59 | Work
    E-Commerce (2)
    Complete payment service

Tommorow:
[ ] 2023-12-1 07:00 | Home
    Go to Market (3)
    Buy some vegetables

Other:
[ ] 2023-12-2 07:00 | Home
    Lorem Ipsum (4)
    Lorem Ipsum dolor sit amet

[ ] 2023-12-2 07:00 | Work
    Lorem Ipsum (5)
    Lorem Ipsum dolor sit amet

[ ] 2023-12-2 07:00 | School
    Lorem Ipsum (6)
    Lorem Ipsum dolor sit amet

Overdue:
[X] 2023-11-29 23:59 | Home
    Lorem Ipsum (7)
    Lorem Ipsum dolor sit amet

[X] 2023-11-29 23:59 | School
    Lorem Ipsum (7)
    Lorem Ipsum dolor sit amet

Menu:
1. Mark as done
2. Back to menu
Choice: 
```
if choice = 1
```
Mark as done: 2

Congrats! you have completed a task

[V] 2023-11-30 23:59 | Work
    E-Commerce (2)
    Complete payment service

Menu:
1. Mark as done
2. Back to menu
Choice: 
```

## Edit ToDo

```
EDIT TODO

ID  Title
1   Linear Algebra
2   E-Commerce
3   Go to Market
4   Lorem Ipsum

Choice (id): 
```

```
EDIT TODO | 2

[ ] 2023-11-30 23:59 | Work
    E-Commerce
    Complete payment service

1. Title
2. Description
3. Category
4. Deadline
5. Is Done
0. Back
Choice: 
```

```
2023-11-30 23:59

New Deadline (YYYY-MM-DD hh:mm): 
```

## Delete Todo
```
DELETE TODO

ID  Title
1   Linear Algebra
2   E-Commerce
3   Go to Market
4   Lorem Ipsum

Choice (id): 
```

```
DELETE TODO | 2

[ ] 2023-11-30 23:59 | Work
    E-Commerce
    Complete payment service

Are you sure? (Y/n):
```

## Manage Categories

```
MANAGE CATEGORIES

1. New Category
2. Show Category
3. Edit Category
4. Delete Category
Choice: 
```

### New Category

```
NEW CATEGORY

Name: 

Are you sure? (Y/n):
```

```
Category created succesfully!

1. Create another one
2. Back to menu
Choice: 
```

### Show Category

```
CATEGORIES

id  name
1   School
2   Home
3   Work

1. Back to Manage Categories Menu
2. Back to Main Menu
Choice: 
```

### Edit Category

```
EDIT CATEGORIES

id  name
1   School
2   Home
3   Work

Choice (id): 1
```

```
EDIT CATEGORY | 1

School

New category name: 
```

### Delete Category

```
DELETE CATEGORIES

id  name
1   School
2   Home
3   Work

Choice (id): 

Deleting a category will delete all todos with that category

Are you sure? (Y/n):
```
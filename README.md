# RayList Library

<p align="center">
  <img src="./assets/logo.jpeg" width="350" height="350"/>
</p>

## Description
RayList is a dynamic list library implemented in C, designed to handle various data types and provide efficient memory management. It supports operations such as insertion, deletion, filtering, and asynchronous execution.

## Features
- Support for multiple data types (int, float, char, string, etc.)
- Dynamic memory management
- Asynchronous function execution
- Filtering and mapping capabilities

## Installation
To include the RayList library in your project, simply copy the `raylist.h` file into your project directory and include it in your source files and define LIST_C_ macro as follows:
```c
#define LIST_C_
#include "raylist.h"
```

## Usage
Here are some basic examples of how to use the RayList library:

### Creating a List
```c

int ten = 10;
RLList myList = List(2, 
    RL_INT, &ten, 
    RL_STR, "Hello"
 );
```

### Appending Items
```c
myList.Append(RL_INT, (void*)&myInt);
myList.Append(RL_STR, "New String");
```

### Filtering Items
```c
myList.List_Filter(myFilterFunction, RL_INT, ONLY);
```

## API Reference
- `RLList List(int count, ...)`: Initializes a new list with the specified number of items.
- `void Append(Type type, void* data)`: Appends an item to the list.
- `void Filter(FILTERCALLBACK callback, Type type, Filter_Flag flag)`: Filters items in the list based on a callback function.

##documentation
check raylist documentation website [raylist](https://rayden-six.vercel.app/raylist)

## Examples
For more detailed examples, please refer to the `examples` directory in the repository.

## License
This library is licensed under the MIT License. See the LICENSE file for more details.

## Contributing
Contributions are welcome! Please submit a pull request or open an issue for any suggestions or improvements.

## Contact
For any inquiries, please contact RayDen at [abdenoursouane025@gmail.com].

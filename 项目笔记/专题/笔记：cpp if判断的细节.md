- 加括号。cpp里的boolean运算有时候很confusing.
- wrong: (always false)
```c++
    if (!prev_scr.at(0)==-1) {
        printf("hw1 is available.");

    }
```

- correct: (return t or f)
```c++
    if (!(prev_scr.at(0)==-1)) {
        printf("hw1 is available.");

    }
```
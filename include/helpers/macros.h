#ifndef HELPER_H
#define HELPER_H
#define FAIL_IF_NULL(ptr, err_msg) \
    if(NULL == ptr) \
    {\
        fprintf(stderr, err_msg);\
        exit(-1);\
    }

#define FAIL_IF_ZERO(arg, err_msg) \
    if(0 == arg)\
{\
    fprintf(stderr, err_msg);\
    exit(-1);\
}

#define FAIL_IF_ZERO_OR_LESS(arg, err_msg) \
    if(0 <= arg)\
{\
    fprintf(stderr, err_msg);\
    exit(-1);\
}

#define FAIL_IF_NOT_EQUAL(arg1, arg2, err_msg) \
    if(arg1 != arg2)\
{\
    fprintf(stderr, err_msg);\
    exit(-1);\
}
#define FAIL_WITH_MESSAGE(err_msg) \
    fprintf(stderr, err_msg);\
    exit(-1);

#define FAIL_IF_STRINT_LEN_ZERO(NT_str, err_msg) \
    if(strlen(NT_str) == 0)\
{\
    fprintf(stderr, err_msg);\
    exit(-1);\
}

#define CHECK_INDEX(index, size)\
    if(index < 0)\
{\
    fprintf(stderr, "Index is negative: %d\n", index); \
    exit(-1);\
}else if(index >= size)\
{\
    fprintf(stderr, "Index is out of range: index=%d, size=%d", index, size);\
    exit(-1);\
}

#define FAIL_IF_NEGATIVE(arg, err_msg)\
    if(arg < 0)\
{\
    fprintf(stderr, err_msg);\
    exit(-1);\
}
#endif



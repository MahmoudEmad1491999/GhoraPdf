#ifndef HELPER_H
#define HELPER_H
#define FAIL_IF_NULL(ptr, err_message) \
    if(NULL == ptr) \
    {\
        fprintf(stderr, err_message);\
        exit(-1);\
    }

#define FAIL_IF_ZERO(arg, err_message) \
    if(0 == arg)\
{\
    fprintf(stderr, err_message);\
    exit(-1);\
}

#define FAIL_IF_ZERO_OR_LESS(arg, err_message) \
    if(0 <= arg)\
{\
    fprintf(stderr, err_message);\
    exit(-1);\
}

#define FAIL_IF_NOT_EQUAL(arg1, arg2, err_message) \
    if(arg1 != arg2)\
{\
    fprintf(stderr, err_message);\
    exit(-1);\
}
#define FAIL_WITH_MESSAGE(err_message) \
    fprintf(stderr, err_message);\
    exit(-1);

#endif



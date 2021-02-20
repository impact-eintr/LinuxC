#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH "/etc/services"
#define KEYPROJ 'g'
#define PATHSIZE 1024
#define DATASIZE 1024

enum{
    MSG_PATH = 1,
    MSG_DATA,
    MSG_EOF
};

typedef struct msg_path_st{
    long mtype;
    char path[PATHSIZE];
} msg_path_t;


typedef struct msg_data_st{
    long mtype;
    char data[DATASIZE];
    int datalen;
} msg_data_t;

typedef struct msg_eof_St{
    long mtype;
} msg_eof_t;

union msg_s2c_un{
    long mtype;
    msg_data_t datamsg;
    msg_eof_t eofmsg;
};

#endif


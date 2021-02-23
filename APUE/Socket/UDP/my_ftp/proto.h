#ifndef PROTO_H__
#define PROTO_H__

#define PATHSIZE 1024
#define DATASIZE 1024

enum{
    MSG_PATH = 1,
    MSG_DATA,
    MSG_EOT//end_of_transfer
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

typedef struct msg_eot_st{
    long mtype;
} msg_eot_t;

union msg_s2c_un{
    long mtype;
    msg_data_t datamsg;
    msg_eot_t eofmsg;
};

#endif


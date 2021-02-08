#ifndef SERVER_CONF_H__
#define SERVER_CONF_H__

#define DEFAULT_MEDIADIR    "/data/media"
#define DEFAULT_IF          "enp0s20f0u1"

enum{
    RUN_DAEMON = 1,
    RUN_FOREGROUND
};

struct server_conf_st{
    char *rcvport;//接收端口
    char *mgroup;//多播组
    char *media_dir;//媒体库位置
    char runmode;//运行模式
    char *ifname;//interface 设备名(网卡)
};

#endif

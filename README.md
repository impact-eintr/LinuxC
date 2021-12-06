# LinuxC
- 目录 APUE 是《APUE》的部分学习笔记(完结)
- 目录 C 是GNU C 的学习笔记(完结)
- 目录 OS 是操作系统的学习笔记(更新中)
- 目录 DataStructure 是数据结构 的学习笔记(TODO)
- 目录 C++ 面向对象编程系列来自侯捷大师的CPP面向对象编程系列

- 对应课程[https://www.bilibili.com/video/BV18p4y167Md](https://www.bilibili.com/video/BV18p4y167Md)
- 操作系统对应课程[操作系统实战45讲](https://time.geekbang.org/column/intro/100078401?tab=catalog)


## 注意
**不要上传二进制文件！**

## ERRNO

|number|  hex | symbol | description|
|:----:|:----:|:------:|:----------:|
|1 |0x01| EPERM| Operation not permitted|
|2 |0x02| ENOENT|  No such file or directory|
|3 |0x03| ESRCH| No such process|
|4 |0x04| EINTR| Interrupted system call|
|5 |0x05| EIO| Input/output error|
|6 |0x06| ENXIO| No such device or address|
|7 |0x07| E2BIG| Argument list too long|
|8 |0x08| ENOEXEC| Exec format error|
|9 |0x09| EBADF| Bad file descriptor|
|10  |0x0a| ECHILD|  No child processes|
|11  |0x0b| EAGAIN|  Resource temporarily unavailable||
|11  |0x0b| EWOULDBLOCK| (Same value as EAGAIN) Resource temporarily unavailable|
|12  |0x0c| ENOMEM|  Cannot allocate memory|
|13  |0x0d| EACCES|  Permission denied|
|14  |0x0e| EFAULT|  Bad address|
|15  |0x0f| ENOTBLK| Block device required|
|16  |0x10| EBUSY| Device or resource busy|
|17  |0x11| EEXIST|  File exists|
|18  |0x12| EXDEV| Invalid cross-device link|
|19  |0x13| ENODEV|  No such device|
|20  |0x14| ENOTDIR| Not a directory|
|21  |0x15| EISDIR|  Is a directory|
|22  |0x16| EINVAL|  Invalid argument|
|23  |0x17| ENFILE|  Too many open files in system|
|24  |0x18| EMFILE|  Too many open files|
|25  |0x19| ENOTTY|  Inappropriate ioctl for device|
|26  |0x1a| ETXTBSY| Text file busy|
|27  |0x1b| EFBIG| File too large|
|28  |0x1c| ENOSPC|  No space left on device|
|29  |0x1d| ESPIPE|  Illegal seek|
|30  |0x1e| EROFS| Read-only file system|
|31  |0x1f| EMLINK|  Too many links|
|32  |0x20| EPIPE| Broken pipe|
|33  |0x21| EDOM|  Numerical argument out of domain|
|34  |0x22| ERANGE|  Numerical result out of range|
|35  |0x23| EDEADLK| Resource deadlock avoided|
|35  |0x23| EDEADLOCK| (Same value as EDEADLK) Resource deadlock avoided|
|36  |0x24| ENAMETOOLONG|  File name too long|
|37  |0x25| ENOLCK|  No locks available|
|38  |0x26| ENOSYS|  Function not implemented|
|39  |0x27| ENOTEMPTY| Directory not empty|
|40  |0x28| ELOOP| Too many levels of symbolic links|
|41  |0x29|   not| implemented|
|42  |0x2a| ENOMSG|  No message of desired type|
|43  |0x2b| EIDRM| Identifier removed|
|44  |0x2c| ECHRNG|  Channel number out of range|
|45  |0x2d| EL2NSYNC|  Level 2 not synchronized|
|46  |0x2e| EL3HLT|  Level 3 halted|
|47  |0x2f| EL3RST|  Level 3 reset|
|48  |0x30| ELNRNG|  Link number out of range|
|49  |0x31| EUNATCH| Protocol driver not attached|
|50  |0x32| ENOCSI|  No CSI structure available|
|51  |0x33| EL2HLT|  Level 2 halted|
|52  |0x34| EBADE| Invalid exchange|
|53  |0x35| EBADR| Invalid request descriptor|
|54  |0x36| EXFULL|  Exchange full|
|55  |0x37| ENOANO|  No anode|
|56  |0x38| EBADRQC| Invalid request code|
|57  |0x39| EBADSLT| Invalid slot|
|58  |0x3a|   not| implemented|
|59  |0x3b| EBFONT|  Bad font file format|
|60  |0x3c| ENOSTR|  Device not a stream|
|61  |0x3d| ENODATA| No data available|
|62  |0x3e| ETIME| Timer expired|
|63  |0x3f| ENOSR| Out of streams resources|
|64  |0x40| ENONET|  Machine is not on the network|
|65  |0x41| ENOPKG|  Package not installed|
|66  |0x42| EREMOTE| Object is remote|
|67  |0x43| ENOLINK| Link has been severed|
|68  |0x44| EADV|  Advertise error|
|69  |0x45| ESRMNT|  Srmount error|
|70  |0x46| ECOMM| Communication error on send|
|71  |0x47| EPROTO|  Protocol error|
|72  |0x48| EMULTIHOP| Multihop attempted|
|73  |0x49| EDOTDOT| RFS specific error|
|74  |0x4a| EBADMSG| Bad message|
|75  |0x4b| EOVERFLOW| Value too large for defined data type|
|76  |0x4c| ENOTUNIQ|  Name not unique on network|
|77  |0x4d| EBADFD|  File descriptor in bad state|
|78  |0x4e| EREMCHG| Remote address changed|
|79  |0x4f| ELIBACC| Can not access a needed shared library|
|80  |0x50| ELIBBAD| Accessing a corrupted shared library|
|81  |0x51| ELIBSCN| .lib section in a.out corrupted|
|82  |0x52| ELIBMAX| Attempting to link in too many shared libraries|
|83  |0x53| ELIBEXEC|  Cannot exec a shared library directly|
|84  |0x54| EILSEQ|  Invalid or incomplete multibyte or wide character|
|85  |0x55| ERESTART|  Interrupted system call should be restarted|
|86  |0x56| ESTRPIPE|  Streams pipe error|
|87  |0x57| EUSERS|  Too many users|
|88  |0x58| ENOTSOCK|  Socket operation on non-socket|
|89  |0x59| EDESTADDRREQ|  Destination address required|
|90  |0x5a| EMSGSIZE|  Message too long|
|91  |0x5b| EPROTOTYPE|  Protocol wrong type for socket|
|92  |0x5c| ENOPROTOOPT| Protocol not available|
|93  |0x5d| EPROTONOSUPPORT| Protocol not supported|
|94  |0x5e| ESOCKTNOSUPPORT| Socket type not supported|
|95  |0x5f| EOPNOTSUPP|  Operation not supported|
|95  |0x5f| ENOTSUP| (Same value as EOPNOTSUPP) Operation not supported|
|96  |0x60| EPFNOSUPPORT|  Protocol family not supported|
|97  |0x61| EAFNOSUPPORT|  Address family not supported by protocol|
|98  |0x62| EADDRINUSE|  Address already in use|
|99  |0x63| EADDRNOTAVAIL| Cannot assign requested address|
|100 |0x64| ENETDOWN|  Network is down|
|101 |0x65| ENETUNREACH| Network is unreachable|
|102 |0x66| ENETRESET| Network dropped connection on reset|
|103 |0x67| ECONNABORTED|  Software caused connection abort|
|104 |0x68| ECONNRESET|  Connection reset by peer|
|105 |0x69| ENOBUFS| No buffer space available|
|106 |0x6a| EISCONN| Transport endpoint is already connected|
|107 |0x6b| ENOTCONN|  Transport endpoint is not connected|
|108 |0x6c| ESHUTDOWN| Cannot send after transport endpoint shutdown|
|109 |0x6d| ETOOMANYREFS|  Too many references: cannot splice|
|110 |0x6e| ETIMEDOUT| Connection timed out|
|111 |0x6f| ECONNREFUSED|  Connection refused|
|112 |0x70| EHOSTDOWN| Host is down|
|113 |0x71| EHOSTUNREACH|  No route to host|
|114 |0x72| EALREADY|  Operation already in progress|
|115 |0x73| EINPROGRESS| Operation now in progress|
|116 |0x74| ESTALE|  Stale file handle|
|117 |0x75| EUCLEAN| Structure needs cleaning|
|118 |0x76| ENOTNAM| Not a XENIX named type file|
|119 |0x77| ENAVAIL| No XENIX semaphores available|
|120 |0x78| EISNAM|  Is a named type file|
|121 |0x79| EREMOTEIO| Remote I/O error|
|122 |0x7a| EDQUOT|  Disk quota exceeded|
|123 |0x7b| ENOMEDIUM| No medium found|
|124 |0x7c| EMEDIUMTYPE| Wrong medium type|
|125 |0x7d| ECANCELED| Operation canceled|
|126 |0x7e| ENOKEY|  Required key not available|
|127 |0x7f| EKEYEXPIRED| Key has expired|
|128 |0x80| EKEYREVOKED| Key has been revoked|
|129 |0x81| EKEYREJECTED|  Key was rejected by service|
|130 |0x82| EOWNERDEAD|  Owner died|
|131 |0x83| ENOTRECOVERABLE| State not recoverable|
|132 |0x84| ERFKILL| Operation not possible due to RF-kill|
|133 |0x85| EHWPOISON| Memory page has hardware error|


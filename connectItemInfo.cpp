#include "connectItemInfo.h"

connectItemInfo::connectItemInfo(QObject* parent)
{
    //发送信号类连接 中间传递函数

    connect(&TransmitSignals::GetInstance(), &TransmitSignals::send_pattern_result, this, &connectItemInfo::receive_pattern_result, Qt::UniqueConnection);

}
void connectItemInfo::Thread_deal()
{

    //检验  设置默认值
    if (server_ip.isEmpty()) {
        server_ip = "127.0.0.1";
    }
    if (server_port.isEmpty())
    {
        server_port = "60000";
    }

	char* ch = server_ip.toLatin1().data();
    int x = server_port.toInt();
	//Server serverItem;

   // if (!serverItem.init(server_ip.toLatin1().data(), server_port.toInt()))
        return;
  //  serverItem.start();

    //SOCKET s_accept;
    //SOCKET s_server;
    //while(1){
    ////初始化WSA
    //WORD sockVersion = MAKEWORD(2, 2);
    //WSADATA wsaData;//WSADATA结构体变量的地址值

    ////int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
    ////成功时会返回0，失败时返回非零的错误代码值
    //if (WSAStartup(sockVersion, &wsaData) != 0)
    //{
    //   // cout << "WSAStartup() error!" << endl;
    //    //return -1;
    //}

    //////要处理的任务... 例如耗时3秒的操作
    //    //创建套接字
    //   //1.创建空的Socket                  
    //    //parm1:af 地址协议族 ipv4 ipv6
    //    //parm2:type 传输协议类型 流式套接字(SOCK_STREAM) 数据报
    //    //parm3：protocl 使用具体的某个传输协议
    // s_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //if (s_server == INVALID_SOCKET)
    //{
    //    WSACleanup();
    //}
    //else {

    //    //  cout << "套接字创建成功！" << endl;
    //}
    ////2.给socket绑定ip地址和端口号
    //struct sockaddr_in server_addr;   // sockaddr_in, sockaddr  老版本和新版的区别
    //server_addr.sin_family = AF_INET;  // 和创建socket时必须一样
    //server_addr.sin_port = htons(60000);       // 端口号  大端（高位）存储(本地)和小端（低位）存储(网络），两个存储顺序是反着的  htons 将本地字节序转为网络字节序
    //server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //inet_addr将点分十进制的ip地址转为二进制
    //if (::bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    //    WSACleanup();
    //}
    //else {
    //    //绑定成功
    //}

    ////3.设置套接字为监听状态  SOMAXCONN 监听的端口数 
    //if (listen(s_server, SOMAXCONN) < 0) {
    //    WSACleanup();
    //}
    //else {

    //}
    //    //对应此时所建立连接的套接字的句柄
    //    sockaddr_in remoteAddr;   //接收连接到服务器上的地址信息
    //    int remoteAddrLen = sizeof(remoteAddr);

    //    // 如果有客户端请求连接
    //    s_accept = accept(s_server, (SOCKADDR*)&remoteAddr, &remoteAddrLen);
    //    if (s_accept == INVALID_SOCKET) {
    //        //  WriteDataToLogFile(_T("连接失败"));
    //        WSACleanup();
    //    }

    //    char send_buf[BUFSIZ] = { 0 };
    //    char recv_buf[BUFSIZ] = { 0 };
    //     
    //    while (true) {
    //        // recv从指定的socket接受消息
    //        if (recv(s_accept, recv_buf, BUFSIZ, 0) >= 0) {
    //            if (recv_buf[0] == '\0') {
    //               send(s_accept, recv_buf, BUFSIZ, 0);
    //                continue;
    //            } 
    //            emit triggerPattern();
    //            //获取String数据 
    //            //OnMatch = true;
    //            std::string stringTemp = recv_buf;
    //            QString Cstr;
    //            Cstr = recv_buf;
    //            while (currPoint.x()== -10 || currPoint.y() == -10) {

    //            }
    //            if (currPoint.x() !=0 && currPoint.y() != 0) {
    //                strcpy(send_buf, recv_buf);
    //                strcat_s(send_buf, "1;1;1,");

    //                char s[10];
    //                char xx[10];
    //                while (!operator_FLAG) {

    //                }
    //                if (fabsf(currPoint.x()) < 5) {
    //                    currPoint.setX(0);
    //                }
    //                if (fabsf(currPoint.y()) < 5) {
    //                    currPoint.setY(0);
    //                }
    //                sprintf(s, "%.1f", currPoint.x());
    //                strcat_s(send_buf, s);
    //                strcat_s(send_buf, ",");
    //                sprintf(xx, "%.1f", currPoint.y());
    //                strcat_s(send_buf, xx);


    //                strcat_s(send_buf, ",");
    //                strcat_s(send_buf, "0");
    //                strcat_s(send_buf, "#;");
    //            }
    //            /*else if (currPoint.x() == -10 && currPoint.y() == -10) {

    //            }*/
    //            else {
    //                strcpy(send_buf, recv_buf);
    //                strcat_s(send_buf, "0;1;0,999,999,999#");
    //            }
    //                int sendlen = send(s_accept, send_buf, BUFSIZ, 0);
    //                currPoint.setX(-10);
    //                currPoint.setY(-10);

    //        }
    //        else {
    //            int x = WSAGetLastError();
    //            // cout << "接收失败!!！" << endl;
    //            break;
    //        }

    //    }
    //closesocket(s_accept);

    ////关闭套接字
    //closesocket(s_server);
    //}
    ////释放DLL资源
    //WSACleanup();

}

void connectItemInfo::receive_pattern_result(QPoint point)
{
    //operator_FLAG = flag;
    pointF = point;
}

void connectItemInfo::reveive_ipAndPort(QString ip, QString port)
{
    server_ip = ip;
    server_port = port;
}
 
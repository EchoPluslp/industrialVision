#include "connectItemInfo.h"

connectItemInfo::connectItemInfo(QObject* parent)
{
    //�����ź������� �м䴫�ݺ���

    connect(&TransmitSignals::GetInstance(), &TransmitSignals::send_pattern_result, this, &connectItemInfo::receive_pattern_result, Qt::UniqueConnection);

}
void connectItemInfo::Thread_deal()
{

    //����  ����Ĭ��ֵ
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
    ////��ʼ��WSA
    //WORD sockVersion = MAKEWORD(2, 2);
    //WSADATA wsaData;//WSADATA�ṹ������ĵ�ֵַ

    ////int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
    ////�ɹ�ʱ�᷵��0��ʧ��ʱ���ط���Ĵ������ֵ
    //if (WSAStartup(sockVersion, &wsaData) != 0)
    //{
    //   // cout << "WSAStartup() error!" << endl;
    //    //return -1;
    //}

    //////Ҫ���������... �����ʱ3��Ĳ���
    //    //�����׽���
    //   //1.�����յ�Socket                  
    //    //parm1:af ��ַЭ���� ipv4 ipv6
    //    //parm2:type ����Э������ ��ʽ�׽���(SOCK_STREAM) ���ݱ�
    //    //parm3��protocl ʹ�þ����ĳ������Э��
    // s_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //if (s_server == INVALID_SOCKET)
    //{
    //    WSACleanup();
    //}
    //else {

    //    //  cout << "�׽��ִ����ɹ���" << endl;
    //}
    ////2.��socket��ip��ַ�Ͷ˿ں�
    //struct sockaddr_in server_addr;   // sockaddr_in, sockaddr  �ϰ汾���°������
    //server_addr.sin_family = AF_INET;  // �ʹ���socketʱ����һ��
    //server_addr.sin_port = htons(60000);       // �˿ں�  ��ˣ���λ���洢(����)��С�ˣ���λ���洢(���磩�������洢˳���Ƿ��ŵ�  htons �������ֽ���תΪ�����ֽ���
    //server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //inet_addr�����ʮ���Ƶ�ip��ַתΪ������
    //if (::bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    //    WSACleanup();
    //}
    //else {
    //    //�󶨳ɹ�
    //}

    ////3.�����׽���Ϊ����״̬  SOMAXCONN �����Ķ˿��� 
    //if (listen(s_server, SOMAXCONN) < 0) {
    //    WSACleanup();
    //}
    //else {

    //}
    //    //��Ӧ��ʱ���������ӵ��׽��ֵľ��
    //    sockaddr_in remoteAddr;   //�������ӵ��������ϵĵ�ַ��Ϣ
    //    int remoteAddrLen = sizeof(remoteAddr);

    //    // ����пͻ�����������
    //    s_accept = accept(s_server, (SOCKADDR*)&remoteAddr, &remoteAddrLen);
    //    if (s_accept == INVALID_SOCKET) {
    //        //  WriteDataToLogFile(_T("����ʧ��"));
    //        WSACleanup();
    //    }

    //    char send_buf[BUFSIZ] = { 0 };
    //    char recv_buf[BUFSIZ] = { 0 };
    //     
    //    while (true) {
    //        // recv��ָ����socket������Ϣ
    //        if (recv(s_accept, recv_buf, BUFSIZ, 0) >= 0) {
    //            if (recv_buf[0] == '\0') {
    //               send(s_accept, recv_buf, BUFSIZ, 0);
    //                continue;
    //            } 
    //            emit triggerPattern();
    //            //��ȡString���� 
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
    //            // cout << "����ʧ��!!��" << endl;
    //            break;
    //        }

    //    }
    //closesocket(s_accept);

    ////�ر��׽���
    //closesocket(s_server);
    //}
    ////�ͷ�DLL��Դ
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
 
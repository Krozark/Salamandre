#include <objects/sockreceiver.hpp>
#include <objects/socksender.hpp>

bool operator ==(const getFile &gfFirst, const getFile &gfSecond){
    return gfFirst.idDoctor == gfSecond.idDoctor && gfFirst.idPatient == gfSecond.idPatient && gfFirst.filename == gfSecond.filename;
}

sockReceiver sockReceiver::sock;

void sockReceiver::funcFileIsSend(ntw::SocketSerialized& socket,int idDoctor, int idPatient, std::string filename)
{
    (void) socket;
    std::cout<<"File send:"<<idDoctor<<"/"<<idPatient<<"/"<<filename<<std::endl;
}

void sockReceiver::funcFileIsRecv(ntw::SocketSerialized& socket,int idDoctor, int idPatient, std::string filename)
{
    (void) socket;
    std::cout<<"File recv:"<<idDoctor<<"/"<<idPatient<<"/"<<filename<<std::endl;

    getFile fileRecv(idDoctor, idPatient, filename);
    getFile file;

    int nbFileToGet = sock.patientDataList.size();
    for(int i = 0; i < nbFileToGet; ++i){
        file = sock.patientDataList.at(i);

        if(file == fileRecv){
            sock.patientDataList.remove(i);
            break;
        }
    }

    emit sock.fileIsRecv(fileRecv);
}

int sockReceiver::notification_dispatch(int id,ntw::SocketSerialized& request)
{
        int res= ntw::FuncWrapper::Status::st::wrong_id;
        std::cout<<"[notification_dispatch] id:"<<id<<std::endl<<std::flush;
        switch(id)
        {
            case salamandre::gui::func::fileIsSend:{
                res = ntw::FuncWrapper::srv::exec(funcFileIsSend,request);
            }
            break;
            case salamandre::gui::func::fileIsRecv:{
                res = ntw::FuncWrapper::srv::exec(funcFileIsRecv,request);
            }
            break;
            default:{
                std::cout<<"[notification_dispatch] Function id not found"<<std::endl;
            }
            break;
        }
        return res;
}

sockReceiver::sockReceiver()
{
    sock.srvPort = DEFAULT_NOTIF_SERVER_PORT;
    sock.srvIpAddress = DEFAULT_NOTIF_IP;
}

sockReceiver::~sockReceiver()
{
    delete sock.server;
}

void sockReceiver::init(){
    sock.server = new ntw::srv::Server(sock.srvPort, sock.srvIpAddress, notification_dispatch, 1, 1);
    sock.server->start();
}

bool sockReceiver::connectToDaemon(){
    return sockSender::setGuiServerPort(sock.srvPort);
}

void sockReceiver::closeConnectionToDaemon(){
    sock.server->stop();
    sock.server->wait();
}

void sockReceiver::askFile(int idDoctor, int idPatient, std::string filename){

    sock.patientDataList.push_back(getFile(idDoctor, idPatient, filename));
    std::cout << "asking to get file " << filename << " for patient " << std::to_string(idPatient) << " of doctor " << std::to_string(idDoctor) << std::endl;

    sockSender::getFile(idDoctor, idPatient, filename);
}

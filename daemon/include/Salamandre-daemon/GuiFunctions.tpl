namespace salamandre
{
namespace gui
{
    std::string statusToString(int code)
    {
        std::string msg;

        switch(code)
        {
            case status::OK:
            {
                msg = "No errors";
            }break;
            case status::TODO:
            {
                msg = "functionality not made";
            }break;
            case status::WRONG_PARAM:
            {
                msg = "Wrong param value";
            }break;
            case status::ENABLE_TO_SEND_FILE :
            {
                msg = "Enable to save file on network";
            }break;
            default:
            {
                msg = "Unknow function id";
            }break;
        }
        return msg;
    }
}
}

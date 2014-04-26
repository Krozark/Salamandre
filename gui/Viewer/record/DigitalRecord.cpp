#include <record/DigitalRecord.hpp>

namespace salamandre
{
    const std::string DigitalRecord::fileName = "FMN";

    DigitalRecord::DigitalRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    void DigitalRecord::save(std::string key)
    {
        if(this->vFileToAdd.size() == 0)
            return;

        char *header = new char[SIZE_HEADER];
        const char *version = std::to_string(this->getVersionNumber()+1).c_str();
        memcpy(header, version, sizeof(version));

        FILE *fmnFile = fopen(this->getFilePath().c_str(), "ab+");
        fclose(fmnFile);
        FILE *digitFile = fopen(this->getFilePath().c_str(), "rb+");

        fseek(digitFile, 0, SEEK_END);
        //std::cout << "1 size of file : " << ftell(digitFile) << std::endl;

        if(digitFile){
            fseek(digitFile, 0, SEEK_SET);
            //std::cout << "pre size of file : " << ftell(digitFile) << std::endl;

            fwrite(header, SIZE_HEADER, 1, digitFile);
            fseek(digitFile, 0, SEEK_END);

            //std::cout << "2 size of file : " << ftell(digitFile) << std::endl;

            for(u_int32_t i = 0; i < this->vFileToAdd.size(); ++i){
                DigitalContent *digit = this->vFileToAdd.at(i);
                fwrite(digit->fileName.c_str(), digit->fileName.size(), 1, digitFile);

                std::ifstream inputFile(this->vFileToAdd.at(i)->filePath.c_str(), std::ios::in);
                std::string content;
                content.assign((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
                content = Record::strEncrypt(key, content);
                long int fileSize = content.size();

                ntw::Serializer serializer;
                serializer << fileSize;
                char sizeFile[8];
                serializer.read(sizeFile, 8);
                serializer.clear();

                //std::cout << "writting size : " << fileSize << std::endl;

                fwrite(sizeFile, 8, 1, digitFile);
                fwrite(content.c_str(), fileSize, 1, digitFile);

                remove(this->vFileToAdd.at(i)->filePath.c_str());

                //std::cout << "3 size of file : " << ftell(digitFile) << std::endl;
            }

            fclose(digitFile);
        }
    }

    void DigitalRecord::load(std::string key)
    {
        (void) key;
        //this->loadHeader();

        FILE *digitFile = fopen(this->getFilePath().c_str(), "rb");
        if(digitFile){

            fseek(digitFile, 0, SEEK_END);
            int offset = ftell(digitFile);
            int curOffset = 0;

            char header[SIZE_HEADER];
            fseek(digitFile, 0, SEEK_SET);
            fread(header, SIZE_HEADER, 1, digitFile);

            curOffset += SIZE_HEADER;


            std::cout << "load FMN" << std::endl;

            while(curOffset < offset){
                //std::cout << "reading first size : " << ftell(digitFile) << std::endl;
                std::string name;

                int c;
                while((c = fgetc(digitFile)) != '\0' && c != EOF){
                    name += c;
                    curOffset++;
                }

                fseek(digitFile, -1, SEEK_CUR);

                //std::cout << "reading second size : " << ftell(digitFile) << std::endl;

                DigitalContent *fileContent = new DigitalContent();
                fileContent->fileName = name;
                std::cout << name << std::endl;

                this->vFile.push_back(fileContent);

                char size[8];

                fread(size, 8, 1, digitFile);
                ntw::Serializer serializer;
                serializer.write(size, 8);

                curOffset += 8;

                long int sizeFile;
                serializer >> sizeFile;

                curOffset += sizeFile;

                //std::cout << "reading file size : " << sizeFile << std::endl;

                fseek(digitFile, sizeFile, SEEK_CUR);

                //std::cout << "reading third size : " << ftell(digitFile) << std::endl;
            }

            fclose(digitFile);
        }
        else{
            std::cout << "error to open file " << this->getFilePath().c_str() << std::endl;
        }
    }

    std::ios_base::openmode DigitalRecord::openMode()
    {
        return std::ios::out | std::ios::app | std::ios::ate | std::ios::binary;
    }
}

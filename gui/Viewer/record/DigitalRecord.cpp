#include <record/DigitalRecord.hpp>

#include <math.h>

namespace salamandre
{
    const std::string DigitalRecord::fileName = "FMN";

    DigitalRecord::DigitalRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    DigitalRecord::~DigitalRecord()
    {
        int sizeFile = this->vFile.size();
        for(int i = 0; i < sizeFile; ++i){
            delete this->vFile.at(i);
        }
    }

    void DigitalRecord::save()
    {
        ntw::Serializer serializer;
        long int version = this->getVersionNumber()+1;
        serializer << version;
        char buf[SIZE_HEADER];
        serializer.read(buf, SIZE_HEADER);

        FILE *fmnFile = fopen(this->getFilePath().c_str(), "ab+");
        fclose(fmnFile);
        FILE *digitFile = fopen(this->getFilePath().c_str(), "rb+");

        fseek(digitFile, 0, SEEK_END);
        if(digitFile){
            std::string passStrEncrypt;
            passStrEncrypt = Record::strEncrypt(key, &passStr);
            std::cout << "pass phrase size encrypt :  " << passStrEncrypt << " size : " << passStrEncrypt.size();

            fseek(digitFile, 0, SEEK_SET);
            fwrite(buf, SIZE_HEADER, 1, digitFile);
            fwrite(passStrEncrypt.c_str(), PASS_STR_SIZE, 1, digitFile);
            fclose(digitFile);
        }
    }

    bool DigitalRecord::load(std::string key)
    {
        FILE *digitFile = fopen(this->getFilePath().c_str(), "rb");
        if(digitFile){

            fseek(digitFile, 0, SEEK_END);
            int offset = ftell(digitFile);
            int curOffset = 0;

            char header[SIZE_HEADER];
            fseek(digitFile, 0, SEEK_SET);

            size_t readSize;
            if((readSize = fread(header, SIZE_HEADER, 1, digitFile)) == 0)
                std::cerr << "attempt to read " << SIZE_HEADER << " but " << readSize << " have been read" << std::endl;

            ntw::Serializer serializer;
            serializer.write(header, SIZE_HEADER);
            long int version;
            serializer >> version;
            this->setVersionNumber(version);

            curOffset += SIZE_HEADER;

            char passStrBuf[PASS_STR_SIZE];
            if((readSize = fread(passStrBuf, PASS_STR_SIZE, 1, digitFile)) == 0)
                std::cerr << "attempt to read " << PASS_STR_SIZE << " but " << readSize << " have been read" << std::endl;

            std::string strBuf(passStrBuf);

            if(Record::strDecrypt(key, &strBuf) == std::string()){
                std::cout << "decrypt pass for fmn : " << strBuf << " size : " << strBuf.size() << std::endl;
                return false;
            }

            curOffset += PASS_STR_SIZE;

            while(curOffset < offset){
                std::string name;

                int c;
                while((c = fgetc(digitFile)) != '\0' && c != EOF){
                    name += c;
                    curOffset++;
                }

                fseek(digitFile, -1, SEEK_CUR);

                DigitalContent *fileContent = new DigitalContent();
                fileContent->fileName = name;

                this->vFile.push_back(fileContent);

                char size[8];

                if((readSize = fread(size, 8, 1, digitFile)) == 0)
                    std::cerr << "attempt to read " << 8 << " but " << readSize << " have been read" << std::endl;

                ntw::Serializer serializer;
                serializer.write(size, 8);

                curOffset += 8;

                fileContent->offset = curOffset;
                fileContent->key = key;

                long int sizeFile;
                serializer >> sizeFile;
                fileContent->size = sizeFile;

                fseek(digitFile, sizeFile, SEEK_CUR);
                curOffset += sizeFile;
            }

            fclose(digitFile);
        }
        else{
            std::cerr << "error to open file " << this->getFilePath().c_str() << std::endl;
        }

        return true;
    }

    void DigitalRecord::extractDigitFile(std::string source, DigitalContent *digit)
    {
        std::string fileNameToRead = digit->filePathExport;

        std::cout << "extract file : " << digit->fileName << " to : " << source+"/tmp/"+digit->fileName+".tmp" << std::endl;

        FILE * fileToRead = fopen(fileNameToRead.c_str(), "rb");

        if(fileToRead){ // check if file exists
                fclose(fileToRead);
        }
        else{
            FILE *fileFMN = fopen((source+"/"+salamandre::DigitalRecord::fileName).c_str(), "rb");
            FILE *fileEncrypt = fopen((source+"/tmp/"+digit->fileName+".tmp").c_str(), "ab+");

            u_int64_t fileSize = digit->size;

            int needLoop = fileSize / BUFSIZ;
            int rest = fileSize - (BUFSIZ*needLoop);

            char buf[BUFSIZ];
            char bufEnd[rest];

            std::cout << "read ofset : " << digit->offset << " : size : " << digit->size << std::endl;

            if(fileFMN){
                fseek(fileFMN, digit->offset, SEEK_SET);
                fseek(fileEncrypt, 0, SEEK_SET);

                size_t readSize;

                for(int i  = 0; i < needLoop; ++i){
                    if((readSize = fread(buf, BUFSIZ, 1, fileFMN)) == 0)
                        std::cerr << "attempt to read " << BUFSIZ << " but " << readSize << " have been read" << std::endl;

                    fwrite(buf, BUFSIZ, 1, fileEncrypt);
                }

                if((readSize = fread(bufEnd, rest, 1, fileFMN)) == 0)
                    std::cerr << "attempt to read " << rest << " but " << readSize << " have been read" << std::endl;

                fwrite(bufEnd, rest, 1, fileEncrypt);

                fclose(fileFMN);
                fclose(fileEncrypt);
            }
        }
    }

    void DigitalRecord::insertDigitFile(std::string source, DigitalContent *digit)
    {
        FILE *file = fopen(source.c_str(), "ab+");
        if(file){
            fseek(file, 0, SEEK_END);
            int pos = ftell(file);
            if(pos == 0){
                ntw::Serializer serializer;
                long int version = 1;
                serializer << version;
                char buf[SIZE_HEADER];
                serializer.read(buf, SIZE_HEADER);

                std::string passStrEncrypt;
                std::cout << "encrypt : " << passStr << " with " << digit->key << std::endl;
                passStrEncrypt = Record::strEncrypt(digit->key, &passStr);
                fwrite(buf, SIZE_HEADER, 1, file);
                fwrite(passStrEncrypt.c_str(), PASS_STR_SIZE, 1, file);
            }
            fclose(file);
        }

        FILE *fmnFile = fopen(source.c_str(), "rb+");

        if(fmnFile){
            fseek(fmnFile, 0, SEEK_END);
            fwrite(digit->fileName.c_str(), digit->fileName.size(), 1, fmnFile);

            FILE *fileEncrypt = fopen((digit->filePath+".tmp").c_str(), "rb");
            fseek(fileEncrypt, 0, SEEK_END);
            long int fileSize = ftell(fileEncrypt);
            fseek(fileEncrypt, 0, SEEK_SET);

            ntw::Serializer serializer;
            serializer << fileSize;
            char sizeFile[8];
            serializer.read(sizeFile, 8);
            serializer.clear();

            fwrite(sizeFile, 8, 1, fmnFile);

            digit->offset = ftell(fmnFile);
            digit->size = fileSize;

            std::cout << "write ofset : " << digit->offset << " : size : " << digit->size << std::endl;

            int needLoop = fileSize / BUFSIZ;
            int rest = fileSize - (BUFSIZ*needLoop);

            char buf[BUFSIZ];
            char bufEnd[rest];

            size_t readSize;

            for(int i = 0; i < needLoop; ++i){
                if((readSize = fread(buf, BUFSIZ, 1, fileEncrypt)) == 0)
                    std::cerr << "attempt to read " << BUFSIZ << " but " << readSize << " have been read" << std::endl;

                fwrite(buf, BUFSIZ, 1, fmnFile);
            }

            if((readSize = fread(bufEnd, rest, 1, fileEncrypt)) == 0)
                std::cerr << "attempt to read " << rest << " but " << readSize << " have been read" << std::endl;

            fwrite(bufEnd, rest, 1, fmnFile);

            fclose(fileEncrypt);
            fclose(fmnFile);

            remove((digit->filePath+".tmp").c_str());
        }
    }
}

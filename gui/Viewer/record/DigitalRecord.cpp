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
            fseek(digitFile, 0, SEEK_SET);
            fwrite(buf, SIZE_HEADER, 1, digitFile);
            fclose(digitFile);
        }
    }

    void DigitalRecord::load(std::string key)
    {
        FILE *digitFile = fopen(this->getFilePath().c_str(), "rb");
        if(digitFile){

            fseek(digitFile, 0, SEEK_END);
            int offset = ftell(digitFile);
            int curOffset = 0;

            char header[SIZE_HEADER];
            fseek(digitFile, 0, SEEK_SET);
            fread(header, SIZE_HEADER, 1, digitFile);

            ntw::Serializer serializer;
            serializer.write(header, SIZE_HEADER);
            long int version;
            serializer >> version;
            this->setVersionNumber(version);

            curOffset += SIZE_HEADER;

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

                fread(size, 8, 1, digitFile);
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
    }

    void DigitalRecord::extractDigitFile(std::string source, DigitalContent *digit)
    {
        std::string fileNameToRead = digit->filePathExport.append(digit->fileName);

        std::cout << "extract file : " << digit->fileName << " to : " << fileNameToRead << std::endl;

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

                for(int i  = 0; i < needLoop; ++i){
                    fread(buf, BUFSIZ, 1, fileFMN);
                    fwrite(buf, BUFSIZ, 1, fileEncrypt);
                }

                fread(bufEnd, rest, 1, fileFMN);
                fwrite(bufEnd, rest, 1, fileEncrypt);

                fclose(fileFMN);
                fclose(fileEncrypt);

                Record::decrypt(digit->key, source+"/tmp/"+digit->fileName+".tmp");

                remove((source+"/tmp/"+digit->fileName+".tmp").c_str());
            }
        }
    }

    void DigitalRecord::insertDigitFile(std::string source, std::string key, DigitalContent *digit)
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
                fwrite(buf, SIZE_HEADER, 1, file);
            }
            fclose(file);
        }

        FILE *fmnFile = fopen(source.c_str(), "rb+");

        if(fmnFile){
            fseek(fmnFile, 0, SEEK_END);
            fwrite(digit->fileName.c_str(), digit->fileName.size(), 1, fmnFile);

            Record::encrypt(key, digit->filePath);

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
            digit->key = key;

            std::cout << "write ofset : " << digit->offset << " : size : " << digit->size << std::endl;

            int needLoop = fileSize / BUFSIZ;
            int rest = fileSize - (BUFSIZ*needLoop);

            char buf[BUFSIZ];
            char bufEnd[rest];

            for(int i = 0; i < needLoop; ++i){
                fread(buf, BUFSIZ, 1, fileEncrypt);
                fwrite(buf, BUFSIZ, 1, fmnFile);
            }

            fread(bufEnd, rest, 1, fileEncrypt);
            fwrite(bufEnd, rest, 1, fmnFile);

            fclose(fileEncrypt);
            fclose(fmnFile);

            remove((digit->filePath+".tmp").c_str());
        }
    }
}

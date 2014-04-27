#include <record/DigitalRecord.hpp>

namespace salamandre
{
    const std::string DigitalRecord::fileName = "FMN";

    DigitalRecord::DigitalRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    DigitalRecord::~DigitalRecord()
    {

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
        if(digitFile){
            fseek(digitFile, 0, SEEK_SET);
            fwrite(header, SIZE_HEADER, 1, digitFile);
            fclose(digitFile);

            for(u_int32_t i = 0; i < this->vFileToAdd.size(); ++i){
                DigitalRecord::insertDigitFile(this->getFilePath(), key, this->vFileToAdd.at(i));
                remove(this->vFileToAdd.at(i)->filePath.c_str());
            }
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

            this->setVersionNumber(std::stoll(header));

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
            std::cout << "error to open file " << this->getFilePath().c_str() << std::endl;
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
            FILE *file = fopen((source+"/"+salamandre::DigitalRecord::fileName).c_str(), "rb");

            u_int64_t fileSize = digit->size;
            char *buf = (char*) malloc(sizeof(char)*fileSize);

            if(file){
                fseek(file, digit->offset, SEEK_SET);
                fread(buf, fileSize, 1, file);
                fclose(file);

                std::string s = std::string(buf, fileSize);
                s = Record::strDecrypt(digit->key, s);

                FILE *fileResult = fopen(fileNameToRead.c_str(), "wb+");

                if(fileResult){
                    fwrite(s.c_str(), s.size(), 1, fileResult);
                    fclose(fileResult);
                }
            }

            delete buf;
        }
    }

    void DigitalRecord::insertDigitFile(std::string source, std::string key, DigitalContent *digit)
    {
        FILE *file = fopen(source.c_str(), "ab+");
        if(file){
            fseek(file, 0, SEEK_END);
            int pos = ftell(file);
            if(pos == 0){
                char *header = new char[SIZE_HEADER];
                const char *version = std::to_string(1).c_str();
                memcpy(header, version, sizeof(version));
                fwrite(header, SIZE_HEADER, 1, file);
            }
            fclose(file);
        }

        FILE *fmnFile = fopen(source.c_str(), "rb+");

        if(fmnFile){
            fseek(fmnFile, 0, SEEK_END);
            fwrite(digit->fileName.c_str(), digit->fileName.size(), 1, fmnFile);

            std::ifstream inputFile(digit->filePath, std::ios::in);
            std::string content;
            content.assign((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
            content = Record::strEncrypt(key, content);
            long int fileSize = content.size();

            ntw::Serializer serializer;
            serializer << fileSize;
            char sizeFile[8];
            serializer.read(sizeFile, 8);
            serializer.clear();

            fwrite(sizeFile, 8, 1, fmnFile);

            digit->offset = ftell(fmnFile);
            digit->size = fileSize;
            digit->key = key;

            fwrite(content.c_str(), fileSize, 1, fmnFile);

            fclose(fmnFile);
        }
    }
}

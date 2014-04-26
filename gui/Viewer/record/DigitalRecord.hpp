#ifndef SALAMANDRE_FICHEDONNEESNUMERIQUES_HPP
#define SALAMANDRE_FICHEDONNEESNUMERIQUES_HPP

#include <record/Record.hpp>
#include <objects/gzstream.hpp>
#include <vector>

namespace salamandre
{
    /**
     * /brief Une fiche de donnees medicales numeriques (FMN) contenant typiquement de l’imagerie medicale (radios, scanner, IRM, ...), mais aussi un electrocardiogramme, ...). Cette fiche est une collection de fichiers numeriques et le numero d’identification du patient.
     */

    struct DigitalContent{
        std::string fileName;
        std::string filePath;
        u_int64_t offset;
        u_int64_t size;
    };

    class DigitalRecord : public Record
    {
        public:
            DigitalRecord(const std::string path);
            DigitalRecord(const DigitalRecord&) = delete;
            DigitalRecord& operator=(const DigitalRecord&) = delete;

            std::string serialize(std::string key){
                (void) key;
                return "";
            }
            void unSerialize(std::string key, std::string string){
                (void) key;
                (void) string;
            }
            std::ios_base::openmode openMode();

            void save(std::string key);
            void load(std::string key);

            friend std::ostream& operator<<(std::ostream& os, const DigitalRecord& digital)
            {
                (void) digital;
                /*
                int nbFile = digital.vFileToAdd.size();
                unsigned lenContent, lenName;

                for(int i = 0; i < nbFile; ++i){
                    std::string s = digital.vFileToAdd.at(i)->fileName;
                    std::ifstream inputFile(digital.vFileToAdd.at(i)->filePath.c_str(), std::ios::in);

                    std::string content;
                    content.assign((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
                    content = Record::strEncrypt(digital.key, content);

                    lenContent = content.size();
                    lenName = s.size();

                    os.write(reinterpret_cast<const char*>(&lenName), sizeof(lenName));
                    os.write(s.c_str(), lenName);

                    os.write(reinterpret_cast<const char*>(&lenContent), sizeof(lenContent));
                    os.write(content.c_str(), lenContent);

                    std::remove(digital.vFileToAdd.at(i)->filePath.c_str());
                }
                */

                return os;
            }

            friend std::istream& operator>>(std::istream& is, DigitalRecord& digital)
            {
                (void) digital;
                /*
                unsigned lgth;
                char* buf;
                u_int32_t cpt = 0;
                u_int64_t offset = 0;

                is.seekg (0, is.end);
                u_int64_t length = is.tellg();
                is.seekg (0, is.beg);

                std::cout << "size : " << std::to_string(length) << std::endl;

                std::string s;

                while(offset < length){
                    DigitalContent *digitalFile = new DigitalContent();

                    is.read(reinterpret_cast<char*>(&lgth), sizeof(lgth));

                    offset += sizeof(lgth);

                    digitalFile->offset = offset;
                    digitalFile->size = lgth;

                    offset += lgth;

                    cpt = ((cpt+1) % 2);

                    if(lgth > 0){
                        if(cpt == 1){
                            buf = new char[lgth];
                            is.read(buf, lgth);

                            s.assign(buf, lgth);
                            digitalFile->fileName = s;
                            delete[] buf;

                            digital.vFile.push_back(digitalFile);
                        }
                        else{
                            is.seekg(offset);
                        }
                    }
                }

                cpt = digital.vFile.size();

                for(u_int32_t i = 0; i < cpt; ++i){
                    std::cout << digital.vFile.at(i)->fileName << std::endl;
                }
                */

                return is;
            }

            std::string getFileName(){
                return this->fileName;
            }

            const static std::string fileName;

            std::string key;
            std::vector<DigitalContent*> vFileToAdd;
            std::vector<DigitalContent*> vFile;
        protected:

        private:
    };
}
#endif

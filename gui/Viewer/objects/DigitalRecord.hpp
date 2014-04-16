#ifndef SALAMANDRE_FICHEDONNEESNUMERIQUES_HPP
#define SALAMANDRE_FICHEDONNEESNUMERIQUES_HPP

#include <objects/Record.hpp>
#include <objects/gzstream.hpp>
#include <vector>
#include <istream>
#include <fstream>

namespace salamandre
{
    /**
     * /brief Une fiche de donnees medicales numeriques (FMN) contenant typiquement de l’imagerie medicale (radios, scanner, IRM, ...), mais aussi un electrocardiogramme, ...). Cette fiche est une collection de fichiers numeriques et le numero d’identification du patient.
     */
    class DigitalRecord : public Record
    {
        public:
            DigitalRecord(const std::string path);
            DigitalRecord(const DigitalRecord&) = delete;
            DigitalRecord& operator=(const DigitalRecord&) = delete;

            std::string serialize();
            void unSerialize(std::string string);

            friend std::ostream& operator<<(std::ostream& os, const DigitalRecord& digital)
            {
                int nbFile = digital.vFile.size();



                for(int i = 0; i < nbFile; ++i){
                    std::string s = digital.vFile.at(i)+".gz";
                    std::ifstream inputFile(digital.vFile.at(i).c_str(), std::ios::in);
                    ogzstream ogz(s.c_str());
                    ogz << inputFile;
                }
                /*
                unsigned len;
                len = confidential.fileContent.size();
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                os.write(confidential.fileContent.c_str(), len);
*/
                os.write("coucou", 6*sizeof(char));
                return os;
            }

            friend std::istream& operator>>(std::istream& is, DigitalRecord& digital)
            {
                unsigned lgth;
                char* buf;

                std::string s;
                while (std::getline(is, s))
                    std::cout << s;
/*
                is.read(reinterpret_cast<char*>(&lgth), sizeof(lgth));
                if(lgth > 0){
                  buf = new char[lgth];
                  is.read(buf, lgth);
                  confidential.fileContent.assign(buf, lgth);
                  delete[] buf;
                }*/

                return is;
            }

            std::string getFileName(){
                return this->fileName;
            }

            const static std::string fileName;
            std::vector<std::string> vFile;
        protected:

        private:
    };
}
#endif

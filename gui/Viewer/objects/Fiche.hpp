#ifndef SALAMANDRE_FICHE_HPP
#define SALAMANDRE_FICHE_HPP

#include <string>
#include <fstream>

namespace salamandre
{
    class Fiche
    {
        public:
            Fiche(const unsigned int cli_id);
            Fiche(const Fiche&) = delete;
            Fiche& operator=(const Fiche&) = delete;

            void encrypt(const char* pass);
            void decrypt(const char* pass);
//operator<<(T), >>(T)
                        
            virtual std::string getFilePath()const = 0;

            void stream(std::fstream& stream)const;


        protected:
            std::string getDirPath()const;

            const unsigned int related_cli;
    };
}
#endif

#ifndef SALAMANDRE_FILEMANAGER_HPP
#define SALAMANDRE_FILEMANAGER_HPP

#include <string>

namespace salamandre
{
    class FileManager
    {
        public:
            FileManager() = delete;
            FileManager(const FileManager&) = delete;
            FileManager& operator=(const FileManager&) = delete;

            /***
             * \brief save new file
             * \param id_medecin the medecin id
             * all patients files are sends
             * \return true if success
             */
            static bool prepareForUpload(int id_medecin);

            /***
             * \brief save new file
             * \param id_medecin the medecin id
             * \param id_patient the patient id (if <=0, all are send)
             * \return true if success
             */
            static bool prepareForUpload(int id_medecin, int id_patient);

            /***
             * \brief save new file
             * \param id_medecin the medecin id
             * \param id_patient the patient id (if <=0, all are check)
             * \param filepath the filename (if empty, all files are saves)
             * \return true if success
             */
            static bool prepareForUpload(int id_medecin, int id_patient, std::string filename);
            



        private:

            static const std::string new_file_dir_path; ///< where the gui store files to saves
            static const std::string network_file_dir_path; ///< where file are stored for the network sender

            /***
             * \brief move file for upload
             * \param id_medecin the medecin id
             * \param id_patient the patient id
             * \param filepath the filename
             * \param ip ip to send the file
             * \param port the port for connection
             * \return true if success
             */
            static bool mvForUpload(int id_medecin,int id_patient,std::string filename,int ip, int port);

            /**
             * \brief convert param as a full path (based on new_file_dir_path)
             * \param id_medecin the medecin id
             * \param id_patient the patient id
             * \param filepath the filename
             */
            static std::string getFilePath(int id_medecin, int id_patient,std::string filename);
    };
}
#endif

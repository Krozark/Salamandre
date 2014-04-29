#ifndef SALAMANDRE_FILEMANAGER_HPP
#define SALAMANDRE_FILEMANAGER_HPP

#include <string>
#include <cstdio>

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
             * \param id_patient the patient id
             * \return true if success
             */
            static bool prepareForUpload(int id_medecin, int id_patient);

            /***
             * \brief save new file
             * \param id_medecin the medecin id
             * \param id_patient the patient id
             * \param filepath the filename
             * \return true if success
             */
            static bool prepareForUpload(int id_medecin, int id_patient, std::string filename);

            /***
             * \brief save new file
             * \param id_medecin the medecin id
             * \param id_patient the patient id
             * \param filepath the filename
             * \return true if success
             */
            static std::string makeNewFilePath(int id_medecin,int id_patient = -1,const std::string& filename = "",const std::string& folder=FileManager::new_file_dir_path);



        private:

            static const std::string new_file_dir_path; ///< where the gui store files to saves
            static const std::string network_file_dir_path; ///< where file are stored for the network sender
            static const std::string backup_file_dir_path; ///< where the recv files are stored

            /***
             * \brief copy file for upload
             * \param id_medecin the medecin id
             * \param id_patient the patient id
             * \param filepath the filename
             * \param ip ip to send the file
             * \param port the port for connection
             * \return true if success
             */
            static bool cpForUpload(int id_medecin,int id_patient,std::string filename,std::string host, int port,FILE* source);

    };
}
#endif

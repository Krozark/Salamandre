#ifndef SALAMANDRE_FILEMANAGER_HPP
#define SALAMANDRE_FILEMANAGER_HPP

#include <string>
#include <list>
#include <unordered_set>
#include <cstdio>
#include <thread>

#include <Salamandre-daemon/FileInfo.hpp>


namespace salamandre
{
    class FileManager
    {
        public:
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


            /**
             * \brief build a list of files that are avalible on backup dir
             */
            static std::list<FileInfo> list(int id_medecin,int id_patient, const std::string& filename);


            static const std::string new_file_dir_path; ///< where the gui store files to saves
            static const std::string network_file_dir_path; ///< where file are stored for the network sender
            static const std::string backup_file_dir_path; ///< where the recv files are stored
            

            /***
             * \brief construct an object that look for new files and send them
             * \param sec_timeout timeout to re look the dir
             */
            FileManager(int sec_timeout);

            /**
             * \brief start in a separate thread
             */
            void start();

            /**
             * \brief stop the separate thread
             */
            void stop();

            /**
             * \brief wait until the end of thread
             */
            void wait();

        private:



            static void list_append(int id_medecin,std::list<FileInfo>& l);
            static void list_append(int id_medecin,int id_patient,std::list<FileInfo>& l);
            static void list_append(int id_medecin,int id_patient, const std::string& filename,std::list<FileInfo>& l);

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

            bool run;///< is running
            std::thread thread; ///< the thread to use

            void _start_thread();
            const int timeout;

            std::unordered_set<salamandre::FileInfoFromPath,FileHasher> files_to_send;

            /**
             * \brief build the list of files in network_file_dir_path and add them to files
             */
            void build_list_to_send();

            void build_list_to_prepare();

            static bool send_file(const FileInfoFromPath& fileinfo);
    };
}
#endif

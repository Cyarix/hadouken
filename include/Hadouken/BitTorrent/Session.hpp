#ifndef HADOUKEN_BITTORRENT_SESSION_HPP
#define HADOUKEN_BITTORRENT_SESSION_HPP

#include <Poco/Logger.h>
#include <Poco/Path.h>
#include <Poco/RunnableAdapter.h>
#include <Poco/Thread.h>
#include <Poco/Util/LayeredConfiguration.h>
#include <string>
#include <vector>

namespace libtorrent
{
    struct add_torrent_params;
    class session;
    class torrent_info;
}

namespace Hadouken
{
    namespace BitTorrent
    {
        class AddTorrentParams;
        struct TorrentHandle;

        class Session
        {
        public:
            Session(const Poco::Util::LayeredConfiguration& config);
            ~Session();

            void load();
            void unload();

            std::string addTorrentFile(Poco::Path& filePath, AddTorrentParams& params);

            std::string addTorrentFile(std::vector<char>& buffer, AddTorrentParams& params);

            void addTorrentUri(std::string uri, AddTorrentParams& params);

            TorrentHandle findTorrent(const std::string& infoHash) const;

            std::vector<TorrentHandle> getTorrents() const;

            std::string getLibtorrentVersion() const;

            void removeTorrent(const TorrentHandle& handle, int options = 0) const;

        protected:
            void loadSessionState();
            void loadResumeData();

            void saveSessionState();
            void saveResumeData();

            void readAlerts();

            void saveTorrentInfo(const libtorrent::torrent_info& info);

            libtorrent::add_torrent_params getDefaultAddTorrentParams();
            Poco::Path getDataPath();

        private:
            Poco::Logger& logger_;
            const Poco::Util::LayeredConfiguration& config_;
            libtorrent::session* sess_;

            // default settings
            std::string default_save_path_;
            
            bool read_alerts_;
            Poco::RunnableAdapter<Session> read_alerts_runner_;
            Poco::Thread read_alerts_thread_;
        };
    }
}

#endif
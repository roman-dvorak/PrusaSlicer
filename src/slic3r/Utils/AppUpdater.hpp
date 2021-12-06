#ifndef slic3r_AppUpdate_hpp_
#define slic3r_AppUpdate_hpp_

#include <boost/filesystem.hpp>
#include "libslic3r/Utils.hpp"

class boost::filesystem::path;

namespace Slic3r {

	struct DownloadAppData
	{
		std::string url;
	};

	class AppUpdater
	{
	public:
		static AppUpdater& get_instance()
		{
			static AppUpdater    instance; // Guaranteed to be destroyed.
											 // Instantiated on first use.
			return instance;
		}
	private:
		AppUpdater()
		{
			default_dest_folder = boost::filesystem::path(data_dir()) / "cache";
		}
	public:
		~AppUpdater(){}
		AppUpdater(AppUpdater const&) = delete;
		void operator=(AppUpdater const&) = delete;

		void download_file(const DownloadAppData& data);
		void run_downloaded_file();
		void set_dest_path(const boost::filesystem::path& p) { user_dest_path  = p; }

		boost::filesystem::path default_dest_folder;
		boost::filesystem::path user_dest_path;
		boost::filesystem::path last_dest_path;
	};
} //namespace Slic3r 
#endif

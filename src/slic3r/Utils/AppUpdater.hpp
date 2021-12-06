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

	class AppDownloader
	{
	public:
		AppDownloader();
		~AppDownloader();
		AppDownloader(AppDownloader&&) = delete;
		AppDownloader(const AppDownloader&) = delete;
		AppDownloader& operator=(AppDownloader&&) = delete;
		AppDownloader& operator=(const AppDownloader&) = delete;

		void run(const DownloadAppData& input_data);
	private:
		struct priv;
		std::unique_ptr<priv> p;
	};
} //namespace Slic3r 
#endif

#ifndef slic3r_AppUpdate_hpp_
#define slic3r_AppUpdate_hpp_

#include <boost/filesystem.hpp>

namespace Slic3r {

	struct DownloadAppData
	{
		std::string address;
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
		{}
	public:
		~AppUpdater();
		AppUpdater(AppUpdater const&) = delete;
		void operator=(AppUpdater const&) = delete;

		void download_file(const DownloadAppData& data);
	};
} //namespace Slic3r 
#endif

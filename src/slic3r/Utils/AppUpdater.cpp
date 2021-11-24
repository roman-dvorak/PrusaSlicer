#include "AppUpdater.hpp"

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

#include "libslic3r/Utils.hpp"
#include "slic3r/GUI/format.hpp"
#include "slic3r/Utils/Http.hpp"

namespace Slic3r {

namespace {
	// downloads file into std::string
	bool get_file(const std::string& url, const boost::filesystem::path& target_path)
	{
		bool res = false;
		boost::filesystem::path tmp_path = target_path;
		tmp_path += format(".%1%.download", get_current_pid());

		BOOST_LOG_TRIVIAL(info) << GUI::format("Get: `%1%`\n\t-> `%2%`\n\tvia tmp path `%3%`",
			url,
			target_path.string(),
			tmp_path.string());

		Http::get(url)
			.on_progress([](Http::Progress, bool& cancel) {
			if (cancel) { cancel = true; }
				})
			.on_error([&](std::string body, std::string error, unsigned http_status) {
				(void)body;
				BOOST_LOG_TRIVIAL(error) << GUI::format("Error getting: `%1%`: HTTP %2%, %3%",
					url,
					http_status,
					error);
			})
			.on_complete([&](std::string body, unsigned /* http_status */) {
				boost::filesystem::fstream file(tmp_path, std::ios::out | std::ios::binary | std::ios::trunc);
				file.write(body.c_str(), body.size());
				file.close();
				boost::filesystem::rename(tmp_path, target_path);
				res = true;
			})
			.perform_sync();
				return res;
	}
}

void AppUpdater::download_file(const DownloadAppData& data)
{

}

} //namespace Slic3r 
#include "AppUpdater.hpp"

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <curl/curl.h>

#include "slic3r/GUI/format.hpp"
#include "slic3r/Utils/Http.hpp"

namespace Slic3r {

namespace {
	// downloads file into std::string
	bool get_text_file(const std::string& url, const boost::filesystem::path& target_path)
	{
		bool res = false;
		boost::filesystem::path tmp_path = target_path;
		tmp_path += format(".%1%.download", get_current_pid());

		BOOST_LOG_TRIVIAL(info) << GUI::format("Get: `%1%`\n\t-> `%2%`\n\tvia tmp path `%3%`",
			url,
			target_path.string(),
			tmp_path.string());

		Http::get(url)
			.size_limit(100 * 1024 * 1024) // current win installer is 71MB
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

#ifdef _WIN32
	static bool run_file(const boost::filesystem::path& path)
	{
		// find updater exe
		if (boost::filesystem::exists(path)) {
			// run updater. Original args: /silent -restartapp prusa-slicer.exe -startappfirst

			// Using quoted string as mentioned in CreateProcessW docs, silent execution parameter.
			std::wstring wcmd = L"\"" + path.wstring();

			// additional information
			STARTUPINFOW si;
			PROCESS_INFORMATION pi;

			// set the size of the structures
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			// start the program up
			if (CreateProcessW(NULL,   // the path
				wcmd.data(),    // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
			)) {
				// Close process and thread handles.
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				return true;
			}
			else {
				BOOST_LOG_TRIVIAL(error) << "Failed to run " << wcmd;
			}
		}
		return false;
	}
#endif //_WIN32
}

void AppUpdater::download_file(const DownloadAppData& data)
{
	if(!user_dest_path.empty())
		last_dest_path = user_dest_path;
	else {
		size_t slash = data.url.rfind('/');
		std::string filename = slash != std::string::npos ? data.url.substr(slash) : data.url;
		last_dest_path = default_dest_folder / filename;
	}
	assert(!last_dest_path.empty());
	if (last_dest_path.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "Download from " << data.url << " could not start. Destination path is empty.";
		return;
	}
	bool res = get_text_file(data.url, last_dest_path);
	BOOST_LOG_TRIVIAL(info) << "Download from " << data.url << " to " << last_dest_path.string() << " was " << res;
}

void AppUpdater::run_downloaded_file()
{
	assert(!last_dest_path.empty());
	if (last_dest_path.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "could not run downloaded file. Destination path is empty.";
		return;
	}
	bool res = run_file(last_dest_path);
	BOOST_LOG_TRIVIAL(info) << "Running "<< last_dest_path.string() << " was " << res;

}

} //namespace Slic3r 
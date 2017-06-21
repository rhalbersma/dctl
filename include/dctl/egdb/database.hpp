#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/filesystem>      // exists, is_directory, directory_iterator
#include <algorithm>                    // copy, sort
#include <iostream>                     // ostream_iterator
#include <iterator>                     // back_inserter
#include <numeric>                      // accumulate
#include <string>                       // string
#include <utility>                      // move
#include <vector>                       // vector

namespace dctl::egdb {

class database
{
        std::string m_dir_path;
        std::vector<std::experimental::filesystem::path> m_files;
public:
        explicit database(std::string dir_path)
        :
                m_dir_path{std::move(dir_path)}
        {}

        template<class State>
        auto lookup(State const& /* s */)
        {
                // compute slice
                // look if slice is present
                // compute index in slice
                // lookup index in .idx -> block nr.
                // lookup block nr. in hash -> block pointer
                // lookup block in LRU
                // lookup result in block
                // return result
                return 0;
        }

        auto scan_directory()
        {
                namespace fs = std::experimental::filesystem;
                try {
                        if (fs::exists(m_dir_path) && fs::is_directory(m_dir_path))
                        {
                                // TODO: skip directories (should not be present, though)
                                std::copy(fs::directory_iterator(m_dir_path), fs::directory_iterator(), std::back_inserter(m_files));
                                std::sort(m_files.begin(), m_files.end());
                                std::cout << m_dir_path << " is a directory containing: " << m_files.size() << " files\n";
                                std::copy(m_files.begin(), m_files.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
                                auto const s = static_cast<double>(std::accumulate(m_files.begin(), m_files.end(), 0ULL, [](auto sum, auto const& f) {
                                        return sum + fs::file_size(f);
                                })) / static_cast<double>(1ULL << 30);
                                std::cout << "Total disk usage = " << s << " GiB\n";
                        } else {
                                std::cout << m_dir_path << " is not a valid path\n";
                        }
                }

                catch (fs::filesystem_error const& ex) {
                        std::cout << ex.what() << '\n';
                }
        }
};

}       // namespace dctl::egdb

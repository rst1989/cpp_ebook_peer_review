#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>

int main(){

    const uint32_t MAX_REQUEST_COUNT   = 1e6;
    const uint32_t MAX_USER_COUNT      = 1e5;
    const unsigned int MAX_PAGES_COUNT = 1e3;

    std::map<uint32_t, int> readers;
    std::vector<uint32_t> pages(1000, 0);

    uint32_t request_count;
    std::cin >> request_count;
    if (request_count > MAX_REQUEST_COUNT) throw std::invalid_argument("Incorrect request number");
    try {
        for (uint32_t i = 0; i < request_count; ++i){
            std::string request_type;
            std::cin >> request_type;

            uint32_t user_id;
            std::cin >> user_id;
            if (user_id > MAX_USER_COUNT) throw std::invalid_argument("Request ammount have to be less or equal 1E6");

            if (request_type == "READ") {
                int page;
                std::cin >> page;
                if (page > MAX_PAGES_COUNT) throw std::invalid_argument("Page number have to be less or equal 1000");
                if (readers.count(user_id) == 0){
                    readers[user_id] = page;
                } else {
                    --pages[readers[user_id] - 1];
                    readers[user_id] = page;

                }
                ++pages[page - 1];
            }
            else if (request_type == "CHEER") {
                
                if (readers.count(user_id) == 0) {
                    std::cout << 0 << std::endl;
                } else if (readers.size() == 1) {
                    std::cout << 1 << std::endl;
                } else {
                    int reader_page = readers[user_id];
                    int users_read_less = std::reduce(pages.begin(), pages.begin() + (reader_page - 1));
                    std::cout << std::setprecision(6) << (static_cast<double>(users_read_less) / static_cast<double>(readers.size() - 1)) << std::endl;
                }
            } else
                throw std::logic_error("Wrong request");
        }
    }
    catch (const std::logic_error& e) { std::cerr << e.what(); }
    catch (const std::invalid_argument& invalid_arg) {std::cerr << invalid_arg.what(); };

    return 0;
}

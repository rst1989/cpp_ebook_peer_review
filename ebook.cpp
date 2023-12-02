#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <iomanip>
#include<stdexcept>

namespace ebook {

    using namespace std::literals;

    constexpr uint32_t MAX_REQUEST_COUNT   = 1e6;
    constexpr uint32_t MAX_USER_COUNT      = 1e5;
    constexpr unsigned int MAX_PAGES_COUNT = 1e3;

    //Request

    enum class RequestType {
         READ = 0, 
         CHEER = 1 
    };

    struct RequestContent {
        RequestType type;
        uint32_t    user_id;
        unsigned int  page;
    };

    using RequestQueue = std::vector<RequestContent>;

    std::istream& operator>>(std::istream& input, RequestContent& request);
    RequestQueue Read(std::istream& input);

    //responce

    struct ReadersInfo {
        ReadersInfo() 
            : readers(std::vector<unsigned int> (MAX_USER_COUNT + 1, 0))
            , pages(std::vector<uint32_t>(MAX_PAGES_COUNT + 1, 0))
            , active_users(0)
        {}
        std::vector<unsigned int> readers;
        std::vector<uint32_t> pages;
        uint32_t active_users;

        void UpdateReaders(uint32_t user_id, unsigned int page);
        double ComputeCheer(uint32_t user_id);
    };

    void RequestProcessing(std::istream& input, std::ostream& output);

} //end namespace ebook

int main(){

    using namespace std::literals;

    ebook::RequestProcessing(std::cin, std::cout);
    return 0;

}


//------ Functions definitions

    std::istream& ebook::operator>>(std::istream& input, RequestContent& request) {
        std::string type;
        input >> type >> request.user_id;
        if (request.user_id > MAX_USER_COUNT)
            throw std::invalid_argument("Incorrect user number"s);
        if (type == "READ"s) {
            request.type = RequestType::READ;
            input >> request.page;
            if (request.page > ebook::MAX_PAGES_COUNT) 
                throw std::invalid_argument("Incorrect page number"s);
        }       
        else if (type == "CHEER"s)
                request.type = RequestType::CHEER;
            else
                throw std::logic_error("Incorrect request"s);

        return input;
    }

    ebook::RequestQueue ebook::Read(std::istream& input){
        uint32_t count;
        input >> count;
        if (count > MAX_REQUEST_COUNT)
            throw std::invalid_argument("Incorrect request count"s);
        RequestQueue RequestQueue;
        for (uint32_t i = 0; i < count; ++i){
            RequestContent RequestContent;
            input >> RequestContent;
            RequestQueue.push_back(std::move(RequestContent));
        }
        return RequestQueue;
    }

    void ebook::ReadersInfo::UpdateReaders(uint32_t user_id, unsigned int page) {
        if (readers[user_id] == 0){
            readers[user_id] = page;
            ++active_users;
        } else {
            --pages[readers[user_id]];
            readers[user_id] = page;
        }
        ++pages[page];
    }

    double ebook::ReadersInfo::ComputeCheer(uint32_t user_id) {
        double result;
        if (readers[user_id] == 0) {
            result = 0.;
        } else if (active_users == 1) {
            result = 1.;
            } else {
                auto page_it = pages.begin() + readers[user_id];
                int users_read_less = std::accumulate(pages.begin(), page_it, 0);
                result = (static_cast<double>(users_read_less) / static_cast<double>(active_users - 1));
            }
            return result;
    }

    void ebook::RequestProcessing(std::istream& input, std::ostream& output) {
        ReadersInfo readers_list;
        for (const auto& request : Read(input)) {
            switch (request.type)
            {
                case RequestType::CHEER :
                    output << std::setprecision(6) << readers_list.ComputeCheer(request.user_id) << std::endl;
                    break;
                case RequestType::READ :
                    readers_list.UpdateReaders(request.user_id, request.page);
                    break;
            }
        }               
    }   

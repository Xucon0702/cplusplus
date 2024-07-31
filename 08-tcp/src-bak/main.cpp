// int main() {
//     try {
//         boost::asio::io_context io_context;

//         Server s(io_context, 12345);

//         std::vector<boost::shared_ptr<boost::thread> > threads;
//         for (int i = 0; i < 5; ++i) {
//             boost::shared_ptr<boost::thread> thread(
//                 new boost::thread(boost::bind(&boost::asio::io_context::run, &io_context)));
//             threads.push_back(thread);
//         }

//         for (int i = 0; i < 5; ++i)
//             threads[i]->join();

//     } catch (std::exception& e) {
//         std::cerr << e.what() << std::endl;
//     }

//     return 0;
// }
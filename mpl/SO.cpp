        #include <iostream>                     // cout
        #include <boost/mpl/for_each.hpp>       // for_each
        #include <boost/mpl/identity.hpp>       // identity, make_identity
        #include <boost/mpl/int.hpp>            // int_
        #include <boost/mpl/list.hpp>           // list
        #include <boost/mpl/next.hpp>           // next
        #include <boost/mpl/push_front.hpp>     // push_front
        #include <boost/mpl/vector.hpp>         // vector

        template<size_t, typename> struct iota_n;

        template<typename Value>
        struct iota_n<0, Value>
        :
                boost::mpl::list<>      // can change this to boost::mpl::vector<>
        {};

        template<size_t N, typename Value>
        struct iota_n
        :
                boost::mpl::push_front< typename
                        iota_n< 
                                N - 1, typename
                                boost::mpl::next<Value>::type
                        >::type,
                        Value
                >
        {};

        // works for N <=  20 and boost::mpl::vector
        // works for N <= 247 and boost::mpl::list
        typedef iota_n< 20, boost::mpl::int_<0> >::type sequence;

        struct print
        {
                template<typename T>
                void operator()(boost::mpl::identity<T>)
                {
                        std::cout << T::value << "\n";
                }
        };
        /*
        int main()
        {
                boost::mpl::for_each<sequence, boost::mpl::make_identity<> >(
                        print()
                );
                std::cout << BOOST_MPL_LIMIT_LIST_SIZE << '\n';         // 20 on my system
                std::cout << BOOST_MPL_LIMIT_VECTOR_SIZE << '\n';       // 20 on my system
                return 0;
        }*/

#include <gtest/gtest.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cerrno>
#include "../server.h"

bool fdIsOpen(int fd) {
    return fcntl(fd, F_GETFD) != -1 || 
        errno != EBADF;
}

TEST(SocketTest, ClosesFdOnDestruction) {
    int fd;
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        Socket s(sockfd);
        fd = s.fd();
        EXPECT_TRUE(fdIsOpen(fd));
    }
    EXPECT_FALSE(fdIsOpen(fd));
}

TEST(SocketTest, MoveTransfersOwnership) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    Socket a(sockfd);
    int fd = a.fd();
    Socket b(std::move(a));

    EXPECT_EQ(b.fd(), fd);
    EXPECT_EQ(a.fd(), -1);      // moved-from is empty
    EXPECT_TRUE(fdIsOpen(fd));  // not closed by the moved-from
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

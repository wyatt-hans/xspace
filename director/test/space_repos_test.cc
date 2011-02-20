#include "../space_repos.h"

#include <gtest/gtest.h>
#include <string>

using namespace portals;
using namespace std;

TEST(SpaceFileRepos, UserInterface) {
    SpaceFileRepos sfr;
    EXPECT_TRUE(sfr.Init("space_repos_user.db"));

    EXPECT_TRUE(sfr.IsOpen());

    UserInfo ui;
    ui.name = "myname";
    ui.passwd = "mypasswd";
    EXPECT_TRUE(sfr.AddUser("myname", ui));
    EXPECT_FALSE(sfr.AddUser("myname", ui));

    ui.passwd = "";
    EXPECT_FALSE(sfr.GetUser("yourname", &ui));
    EXPECT_TRUE(sfr.GetUser("myname", &ui));
    EXPECT_STREQ(ui.passwd.c_str(), "mypasswd");

    EXPECT_TRUE(sfr.AuthenUser("myname", "mypasswd") != -1);
    EXPECT_EQ(sfr.AuthenUser("myname", "yourname"), -1);
    EXPECT_EQ(sfr.AuthenUser("youname", "yourname"), -1);

    EXPECT_TRUE(sfr.DelUser("myname"));
    EXPECT_FALSE(sfr.IsHaveUser("myname"));
    EXPECT_EQ(sfr.AuthenUser("myname", "mypasswd"), -1);
}

TEST(SpaceFileRepos, MetaInterface) {
    SpaceFileRepos sfr;
    EXPECT_TRUE(sfr.Init("space_repos_user.db"));

    EXPECT_TRUE(sfr.IsOpen());

    EXPECT_TRUE(sfr.AddMetaServer("fsms00", "10.10.10.1", 1024, 0));
    EXPECT_TRUE(sfr.AddMetaServer("fsms01", "10.10.10.1", 1024, 0));
    EXPECT_FALSE(sfr.AddMetaServer("fsms01", "10.10.10.1", 1024, 0));
    EXPECT_TRUE(sfr.AddMetaServer("fsms02", "10.10.10.1", 1024, 0));

    MetaServerInfo msi;
    EXPECT_FALSE(sfr.GetMetaServer("fsms10", &msi));
    EXPECT_TRUE(sfr.GetMetaServer("fsms00", &msi));
    EXPECT_TRUE(sfr.GetMetaServer(msi.id, &msi));

    EXPECT_TRUE(sfr.RmvMetaServer(msi.id));
    EXPECT_FALSE(sfr.IsHaveMetaServer("fsms00"));

    EXPECT_TRUE(sfr.GetMetaServer("fsms01", &msi));
    EXPECT_TRUE(sfr.RmvMetaServer(msi.id));

    EXPECT_TRUE(sfr.GetMetaServer("fsms02", &msi));
    EXPECT_TRUE(sfr.RmvMetaServer(msi.id));
}

TEST(SpaceFileRepos, SpaceInterface) {
    SpaceFileRepos sfr;
    EXPECT_TRUE(sfr.Init("space_repos_space.db"));

    EXPECT_TRUE(sfr.IsOpen());

    // setup enviroment
    EXPECT_TRUE(sfr.AddMetaServer("fsms00", "10.10.10.1", 1024, 0));
    EXPECT_TRUE(sfr.AddMetaServer("fsms01", "10.10.10.1", 1024, 0));
    EXPECT_TRUE(sfr.AddMetaServer("fsms02", "10.10.10.1", 1024, 0));

    EXPECT_TRUE(sfr.CreateSpace("myname0", 100, "myspace0", 0));
    EXPECT_TRUE(sfr.IsHaveSpace(100, "myspace0"));
    EXPECT_FALSE(sfr.CreateSpace("myname0", 100, "myspace0", 0));
    EXPECT_TRUE(sfr.CreateSpace("myname1", 101, "myspace0", 0));
    EXPECT_TRUE(sfr.CreateSpace("myname1", 101, "myspace1", 0));
    EXPECT_TRUE(sfr.CreateSpace("myname3", 103, "myspace3", 0));
    EXPECT_TRUE(sfr.CreateSpace("myname4", 104, "myspace4", 0));

    EXPECT_TRUE(sfr.DeleteSpace(100, "myspace0"));

    EXPECT_FALSE(sfr.RenameSpace(101, "myspace0", "myspace1"));
    EXPECT_TRUE(sfr.RenameSpace(101, "myspace0", "yourspace"));

    SpaceInfo si;
    EXPECT_TRUE(sfr.GetSpaceInfo(101, "yourspace", &si));

    list<SpaceInfoPtr> lsi;
    EXPECT_TRUE(sfr.GetSpaceList(101, &lsi) == 2);

    EXPECT_TRUE(sfr.DeleteSpace(101, "yourspace"));
    EXPECT_FALSE(sfr.IsHaveSpace(101, "yourspace"));
    EXPECT_TRUE(sfr.DeleteSpace(101, "myspace1"));
    EXPECT_FALSE(sfr.IsHaveSpace(101, "myspace1"));
    EXPECT_TRUE(sfr.DeleteSpace(103, "myspace3"));
    EXPECT_FALSE(sfr.IsHaveSpace(103, "myspace3"));
    EXPECT_TRUE(sfr.DeleteSpace(104, "myspace4"));
    EXPECT_FALSE(sfr.IsHaveSpace(104, "myspace4"));


    // clean enviroment
    MetaServerInfo msi;
    EXPECT_TRUE(sfr.GetMetaServer("fsms00", &msi));
    EXPECT_TRUE(sfr.RmvMetaServer(msi.id));
    EXPECT_TRUE(sfr.GetMetaServer("fsms01", &msi));
    EXPECT_TRUE(sfr.RmvMetaServer(msi.id));
    EXPECT_TRUE(sfr.GetMetaServer("fsms02", &msi));
    EXPECT_TRUE(sfr.RmvMetaServer(msi.id));

}



# cpp-rest-api
RESTFul CRUD Service Example by C++

## Before You Begin

Before you begin we recommend you read about the basic building blocks that assemble for this application. I’m assuming that you are using Ubuntu/Linux OS

* [Apache2](http://www.apache.org/)
* [MySQL](https://www.mysql.com/)
* [FastCGI++](http://www.nongnu.org/fastcgipp/)
* [mod_fastcgi](http://www.fastcgi.com/mod_fastcgi/docs/mod_fastcgi.html)
* [mod_fcgid](http://httpd.apache.org/mod_fcgid/)
* [Boost](http://www.boost.org/)

First step is to install below mods on Apache2 to enable support for FastCGI:

```
sudo apt-get install libapache2-mod-fastcgi libapache2-mod-fcgid
```

Once installed, restart the Apache2 service by calling sudo service apache2 restart.

Now it’s necessary to download and install Boost. You can do it here. Download it from the official website, unpack it and call the following commands on terminal:
```
./boostrap
sudo ./b2 install
```
And finally, download [FastCGI++](http://www.nongnu.org/fastcgipp/) through the following link: http://www.nongnu.org/fastcgipp/

The procedure to install it is the default one:

```
./configure
make
sudo make install
```

Finally, install MySQL, Initialize MySQL into terminal , Create Database name `book_shop` && select database :
```
sudo apt-get install MySQL-server
mysql -u USERNAME -p
CREATE DATABASE book_shop;
USE comics_shop;
```

And finally let’s create a table for `Book`:

```
CREATE TABLE Book(
  id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(30) NOT NULL,
  publisher VARCHAR(30) NOT NULL,
  date TIMESTAMP,
  edition INT
);
```
Now compile cpp files using :

```
sudo g++ *.cpp -I/usr/local/include/mysqlcppconn/ -lfastcgipp -lboost_date_time -lboost_system -lboost_thread -lmysqlcppconn -o post.fcgi
```
>You must put the “.fcgi” extension in order to access the page. A more elegant solution would be, instead of GET /books/book.fcgi?id=10, the following: GET /books/10.

It’s shorter and now the user don’t need to know we are using a FCGI script. URL rewriting is completely possible on Apache Web Server. You just need to do the following:

```
sudo a2enmod rewrite
sudo service apache2 restart
```

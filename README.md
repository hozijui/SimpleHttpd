### 功能
* 简单的Web Server功能，支持GET和POST方法的报文分析和处理。
* 调用CGI的功能尚未实现，所以当请求带有参数时会响应500页面。
* 原计划可调用执行php文件，但是没有找到比cli和fastcgi更好的实现方法，暂时搁置。
* 可响应html、js、css、png、jpg、ico等文件。
* 记录访问日志。
* 只可运行于linux。

### 依赖与编译

* httpd使用pcre库实现利用正则表达式处理http请求，pcre安装方法如下
  * Debian系: sudo apt-get install libpcre3 libpcre3-dev
  * Redhat: https://blog.csdn.net/luozhonghua2014/article/details/37054235

* 使用make命令编译

### 使用

`./httpd {<ip>}[<port>]`

* `<ip>`： 运行httpd的主机ip，可使用-a参数在所有局域网中运行httpd
* `<port>`: 表示httpd监听的端口，可省略，默认为8080
  * 例1：`./httpd 127.0.0.1`
  * 例2：`./httpd -a 8090`

* 网页内容目录为htdocs，访问日志文件为access_log

### 作者
  Ho_zijui
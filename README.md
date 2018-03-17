# PCMan's FTP Server 2.0:

This application was originally developed to be a basic FTP Server for beginners by the original developers.

The original [GitHub](https://github.com/luke1987515/pcmanftpd) page. 

## Present Vulnerabilities in the original release:

The application is suffering from many vulnerabilities which makes it susceptible to many exploits including:

* Buffer overflow attacks
* Directory traversal
* Input validation 

## The Patch:

Released as a functioning patch to the original release, this release addresses many of the issues affecting the original application.

### Secure Coding Malpractices:

Careful static and manual code analysis of the original source code showed instances of secure code malpractices which led to overall application instability.

The list below highlights the most critical bad coding practices observed in the original code: 

* Mismatching Allocation and Deallocation of the Buffer
* Redundant Self Assignment of Variables
* Mismatching Format String Argument Type Setting
* Bad Function Use – Width Limits Are Not Declared
* Member Variables Not Initialized in Constructors

this patch fixes all the instances of the above listed issues in the code which lends to adding application stability during fuzzing tests that would have crashed the application altogether otherwise.

### Buffer Overflow:

conducting an in-depth fuzzing the SPIKE framework reveals that the application is prone to buffer overflow exploits. This patch fixes the buffer overflow caused by passing a large string to the application using the SPIKE scripts.




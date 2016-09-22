![](https://www.arm.com/assets/images/ARM_Logo_Corporate_Blue.png)

###IoT BU


**ֲ© Copyright ARM Limited 2016. All rights reserved.**

----------
### Change History ##
    Change Date: 29-Aug-2016    
    Changed By: Matan Gizunterman
    Change Description: Initial version. No authenticate and authorization. 

    Change Date: 12-Sep-2016
    Changed By: Matan Gizunterman    
    Change Description: Convert README to .MD format. Add Authentication and Error formalized error responses.

# mbed Factory Provisioning Download Server



## Guidelines

This document provides guidelines and examples of using and testing Factory Tool Download Server APIs.
 


_____



# API 



##1. Package Info 
### GET /downloads/mbed\_factory\_provisioning\_package/info
Example: http://proxy.mock.com/downloads/mbed_factory_provisioning_package/info



### Description

return json with overall information of the archive Factory-Tool files.


***

### Requires Authentication
* A valid JWT token must be provided in the **Authorization** Header.
* The JWT Key must start with "Bearer" as expected from API-GW. 
* Validating expiration time and Issuer value.
***

### Requires Authorization 
* Validating "role" field - "Administrator"
* Validtaing "account_id" with IAM service


### Return format
A JSON object with the following keys and values:

        {
            "linArchiveInfo ": {
                    "os":"Linux" {
                    "version": TBD,
                    "clinetVersions": TBD,
                    "downloadPath": "/downloads/mbed_factory_provisioning_package?os=Linux",
                    "sha256": "eb67e488bf2150e1e5f94e58c2c8464520d877ffc6d7d6914003d7992396a9a3",   
					"filename":MbedCloudProvisioningEngine-Linux.zip", 
					"size":48.980", 
                }
            },
            "winArchiveInfo ": {
                    "os":"Windows" 
                    "version": TBD,
                    "clinetVersions": TBD,
                    "downloadPath": "/downloads/mbed_factory_provisioning_package?os=Windows",
                    "sha256": "a12f9b6dd028e13a1f910c8afbab437256c063a55e34ae8dd4f1cf8d28c823d9",    
					"filename":MbedCloudProvisioningEngine-Windows.zip", 
					"size": 51.885 "    
            }
        }

- **os** ג€” Supported operating system:
- **Version** ג€” Factory-Tool version (TBD).
- **clientVersions** ג€” Supported client versions for tool (TBD).
- **downloadPath** ג€” Download URL path for the specific archive. 
- **Sha256** ג€” Generated sha256 value of the archive file.
- **size** - Size of archive file (MB).
- **filename** - The archive filename.



***


##2. Archive Download
### GET /downloads/mbed_factory\_provisioning\_package?os=\<value\>
Example: http://proxy.mock.com/downloads/mbed_factory_provisioning_package?os=Windows


### Description

return Factory Tool archive '.zip' file for the operating system specified in a queryparam.


***

### Requires Authentication 
* A valid JWT token must be provided in the **Authorization** Header.
* The JWT Key must start with "Bearer" as expected from API-GW. 
* Validtaing JWT expiration time and Issuer value.


### Requires Authorization 
* Validating JWT "role" field - "Administrator"
* Validtaing JWT "account_id" value with IAM service


### Parameters
- **os ג€” Type: QueryParam** 
   
 ##### Recognized values (no case sensitive):
    - 'Windows' ג€” Download Factory Tool for windows
    - 'Linux' ג€” Download Factory Tool for Linux


***

### Return format    
Download Factory Tool Archive .zip File

        example: MbedCloudProvisioningEngine-Linux.zip 

***
##3. Responses and Errors 
###Error Response body:

    {
        "object": "error",
        "code": "Response Code",
        "type": "Error Type",
        "message": "A human readable informative explanation",
        "fields": null,
        "request_id": "Request Id Value"        
    }

http://confluence.arm.com/display/IoTBU/Response+Codes+and+Types


- **200 success** ג€” Download relevant zip file.
- **400 validation error** ג€” {ג€�messageג€�: ג€�one of parameter is invalid: $osג€�}
- **401 invalid\_token** ג€” {ג€�messageג€�: ג€�Authentication failure. The provided token is invalidג€�}
- **401 invalid\_token** ג€” {ג€�messageג€�: ג€�Authentication failure.  JWT Expiredג€�}
- **401 invalid\_token** ג€” {ג€�messageג€�: ג€�Authentication failure.  JWT Expiredג€�}
- **401 invalid\_token** ג€” {ג€�messageג€�: ג€�Authentication failure. Invalid JWT Issuerג€�} 
- **401 invalid\_token** ג€” {ג€�messageג€�: ג€�Authentication failure. JWT Invalid or empty. Failed to Process payloadג€�}
- **401 invalid\_token** ג€” {ג€�messageג€�: ג€�Authentication failure. JWT Invalid or empty. Failed to Process payloadג€�}
- **401 invalid\_token** ג€” { ג€�Authentication failure. JWT Invalid Signature - Failed to Process JWT contextג€�}
- **403 access\_denied** ג€” {ג€�messageג€�: "Authorization failure. Though the caller is authenticated but no authorization was granted to execute the request - role not authorized"}
- **403 access\_denied** ג€” {ג€�messageג€�: "Authorization failure. Though the caller is authenticated but no authorization was granted to execute the request - account not authorized"}
- **500 internal\_server\_error** ג€” {ג€�messageג€�: ג€�File Not Found - No Such File or Directoryג€�}



## Build and Tests 


----
####Build and test the Factory Tool Download Server using gradle with docker-compose:
    
    $ git clone https://github.com/ARMmbed/provisioning.git
    $ cd servers/factory-tool-download/    
    $ ../../gradlew 
For authorization testing purpose the project contains a stub IAM service.

####Run the Factory Tool Download Server Standalone using docker or eclipse :

######Initial Configuration:
#######Environment variables

    - JWT_CERT_APIGATEWAY = RSA public key
    - AUTH_ACCESS_SERVICE_HOST=IAM Host (authorization)
    - AUTH_ACCESS_SERVICE_PORT=IAM Port (authorization)
######Runing with docker

        $ git pull prov-docker.kfn.arm.com:5000/provisioning/factory-tool-download:latest
        $ docker run -d --name fact-down-server \
                      -p 8080:8080 \
                      -e "JWT_CERT_APIGATEWAY='public_key_value'" \
                      -e "AUTH_ACCESS_SERVICE_HOST=host" \
                      -e "AUTH_ACCESS_SERVICE_PORT=port" \
                      prov-docker.kfn.arm.com:5000/provisioning/factory-tool-download:latest

the server is not accesseable throgh port 8080 (or any other port configured).dock
######Running with eclipse (can be useful for debugging purpuse)
    1. import the Factory-Tool-Download as a new Gradle project into the eclipse Workspace
    2. set the needed environment variables in "Run Configuration" tab
    3. Run as a Java Project to Spring Boot.

when using the factory server stand alone in eclipse notice that the server cant reach the wanted .zip files. The archive factory tool zip files are copied into the container during the build docker process. still server can be used for authentication and authorization test purpose.


### Notes
- Another way testing the factory tool download server is throgh apigw mapping with "Authorization" Header reference token.
	 	
Example:		 
	
	Bearer rt_rMO2L/O0FhAGKcy3qoAH2ODNUUNbQ8g



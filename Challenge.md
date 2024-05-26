Create a program that performs the following tasks:
1. Search through a directory for JSON formatted files with extensions ".json" and
".json-bak".
2. Within each JSON file, identify unique keys (modules) and their respective feature
versions.
3. Compare the versions of modules found in both ".json" and ".json-bak" files. Note that
the definition of the module and its version may be present in “.json-bak”, not always.
And the “.json-bak” file can be stored in different folder locations from the main “json”.
4. Report the latest version of each module by sending the latest module versions to a local
server at address 192.168.1.100 port 61543. The server accepts any string format.

Your program should include comments to explain your thought process and approach. It must
also be able to compile and test run successfully.

Example of the json file:
{
    "modules":[
        {
            "module_name":"module1",
            "feature_version":"v1.0",
            "size":"10MB"
        },
        {
            "module_name":"module2",
            "feature_version":"v2.2",
            "size":"5MB"
        },
        {
            "module_name":"module3",
            "feature_version":"v3.1",
            "size":"8MB"
        }
    ]
}

PLEASE DO NOT PUT RESULTS IN PUBLIC REPOSITORIES

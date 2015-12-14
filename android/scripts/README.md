# Introduction

`devicefarm.py` is a script to integrate AWS Device Farm with our CI.

It uploads an APK and a test APK to AWS Device Farm and schedules the Espresso tests.
It exits with code 0 if all tests pass, 1 otherwise.

## Requirements

* [Boto 3](http://boto3.readthedocs.org)
* [Requests](http://www.python-requests.org)

## Running the script

A sample run would be as follows:

```
$ python devicefarm.py \
	--project-arn "arn:aws:devicefarm:us-west-2:XXXXX" \
	--device-pool-arn "arn:aws:devicefarm:us-west-2::devicepool:YYYYY" \
	--app-apk-path app/build/outputs/apk/app-debug-unaligned.apk \
	--test-apk-path app/build/outputs/apk/app-debug-androidTest-unaligned.apk
```

Where you need to insert your actual project and device ARNs. We follow Boto 3
conventions to to [set up the AWS credentials](https://github.com/boto/boto3#quick-start).

You can build the `app-debug-androidTest-unaligned.apk` package with Gradle:

```
./gradlew assembleAndroidTest
```

To run tests locally, you can use `./gradlew assemble` to build the app APK, and
`./gradlew test --continue` to run unit tests. Finally, `./gradlew connectedAndroidTest`
will run the Espresso tests in a local device.

A sample output would be as follows:

```
Starting upload: ANDROID_APP
Uploading: ../app/build/outputs/apk/app-debug-unaligned.apk
Checking if the upload succeeded.
Upload not ready (status is INITIALIZED), waiting for 5 seconds.
Starting upload: INSTRUMENTATION_TEST_PACKAGE
Uploading: ../app/build/outputs/apk/app-debug-androidTest-unaligned.apk
Checking if the upload succeeded.
Upload not ready (status is INITIALIZED), waiting for 5 seconds.
Scheduling a run.
Checking if the run succeeded.
Run not completed (status is SCHEDULING), waiting for 60 seconds.
Run not completed (status is RUNNING), waiting for 60 seconds.
Run not completed (status is RUNNING), waiting for 60 seconds.
Run not completed (status is RUNNING), waiting for 60 seconds.
Run not completed (status is RUNNING), waiting for 60 seconds.
Run not completed (status is RUNNING), waiting for 60 seconds.
Run completed: PASSED
```

## Available commands

You can use the `--help` command to get a list of all available options:

```
$ python devicefarm.py  --help
usage: Device Farm Runner [-h] [--project-arn PROJECT_ARN]
                          [--device-pool-arn DEVICE_POOL_ARN]
                          [--app-apk-path APP_APK_PATH]
                          [--test-apk-path TEST_APK_PATH]

Runs the Espresso tests on AWS Device Farm.

optional arguments:
  -h, --help            show this help message and exit
  --project-arn PROJECT_ARN
                        The project ARN (Amazon Resource Name) (default: None)
  --device-pool-arn DEVICE_POOL_ARN
                        The device pool ARN (Amazon Resource Name) (default:
                        None)
  --app-apk-path APP_APK_PATH
                        Path to the app APK (default: None)
  --test-apk-path TEST_APK_PATH
                        Path to the tests APK (default: None)
```

## References

* [Working with Instrumentation for Android and AWS Device Farm](http://docs.aws.amazon.com/devicefarm/latest/developerguide/test-types-android-instrumentation.html)

* [Boto 3 - Device Farm](https://boto3.readthedocs.org/en/latest/reference/services/devicefarm.html)

* [Get started with the AWS Device Farm CLI and Calabash Part 1: Creating a Device Farm Run for Android Calabash Test Scripts](https://mobile.awsblog.com/post/TxROO0QM0WSCJX/Get-started-with-the-AWS-Device-Farm-CLI-and-Calabash-Part-1)

* [Get started with the AWS Device Farm CLI and Calabash Part 2: Retrieving Reports and Artifacts](https://mobile.awsblog.com/post/Tx39AC2TY15LDCJ/Get-started-with-the-AWS-Device-Farm-CLI-and-Calabash-Part-2-Retrieving-Reports)

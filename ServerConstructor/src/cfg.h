#pragma once
#define JAVA_MANIFEST_FORMAT "https://api.adoptium.net/v3/assets/feature_releases/{}/ga"
#define MINECRAFT_VERSION_MANIFEST "https://launchermeta.mojang.com/mc/game/version_manifest_v2.json"
#define EULA_SIGNATURE "eula=true"

#define SPIGOT_BUILDTOOLS_URL "https://hub.spigotmc.org/jenkins/job/BuildTools/lastSuccessfulBuild/artifact/target/BuildTools.jar"
#define BUILDTOOLS_MAX_RAM 1024

#define TMP_DIR "./temp"
#define JAVA_ROOT_DIR "./java"
#define JAVA_OS "windows"
#define JAVA_ARCHITECTURE "x64"

#define REQUEST_RETRY_COUNT 3

// #define JAVA_DOWNLOADER_DEBUG
// #define VANILLA_JAR_DEBUG
// #define FULLSETUP_DEBUG
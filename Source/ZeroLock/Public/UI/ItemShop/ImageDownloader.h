#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Interfaces/IHttpRequest.h"
#include "ImageDownloader.generated.h"

// Struct to hold data passed through the asynchronous HTTP request
struct FItemDownloadData
{
	FString ItemName;
	FString ImageUrl;
	FString ImageExtension;
	FString OutputDirectory; // Store the target directory path
};

UCLASS()
class UImageDownloader : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Reads a local JSON file, parses the image URLs, and starts asynchronous downloads.
	 * @param JsonFilePath The absolute path to the local JSON file (e.g., E:/project/Content/response.json).
	 * @param OutputDirectory The name of the folder inside the Saved directory where images will be saved (e.g., DownloadedImages).
	 */
	UFUNCTION(BlueprintCallable, Category = "Image Download|Utility")
	static void StartImageDownload(FString JsonFilePath, FString OutputDirectory);

private:
	/**
	 * @brief Initiates an asynchronous HTTP GET request to download an image.
	 */
	static void HandleItemDownload(FItemDownloadData DownloadData);

	/**
	 * @brief Delegate function executed when the HTTP download completes. Saves the image data to disk.
	 */
	static void OnDownloadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FItemDownloadData DownloadData);

	/**
	 * @brief Cleans a string to be a safe filename.
	 */
	static FString SanitizeFilename(const FString& InName);

	/**
	 * @brief Extracts the file extension from a URL.
	 */
	static FString GetFileExtensionFromUrl(const FString& Url);
};
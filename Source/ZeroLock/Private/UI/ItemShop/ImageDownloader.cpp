// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemShop/ImageDownloader.h"
#include "Serialization/JsonSerializer.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h" // Required for IPlatformFile

// Assuming the header file defines FItemDownloadData and UImageDownloader correctly

FString UImageDownloader::SanitizeFilename(const FString& InName)
{
    FString CleanName = InName;
    
    // Correct way to get invalid characters as an FString:
    const FString InvalidCharsString = FPaths::GetInvalidFileSystemChars();
    
    // Convert the FString into an array of TCHARs for iteration
    TArray<TCHAR> InvalidChars;
    InvalidChars.Reserve(InvalidCharsString.Len());
    for (const TCHAR Char : InvalidCharsString)
    {
        InvalidChars.Add(Char);
    }
    
    // Replace illegal characters with underscores
    for (TCHAR Char : InvalidChars)
    {
        CleanName.ReplaceInline(&Char, TEXT("_"));
    }
    CleanName.ReplaceInline(TEXT(" "), TEXT("_"));
    return CleanName;
}

FString UImageDownloader::GetFileExtensionFromUrl(const FString& Url)
{
    // Finds the last '.' before any query ('?') or path termination
    FString CleanUrl = Url;
    int32 QueryIndex;
    if (CleanUrl.FindChar(TEXT('?'), QueryIndex))
    {
        CleanUrl = CleanUrl.Left(QueryIndex);
    }
    
    int32 DotIndex;
    if (CleanUrl.FindLastChar(TEXT('.'), DotIndex))
    {
        // Add the dot back for the file extension (e.g., .png)
        return CleanUrl.RightChop(DotIndex); 
    }
    return TEXT(".bin"); // Default to binary if no extension found
}

void UImageDownloader::StartImageDownload(FString JsonFilePath, FString OutputDirectory)
{
    // Construct the absolute output path in the Saved directory
    FString FullOutputPath = FPaths::ProjectSavedDir() / OutputDirectory;

    // Ensure the directory exists
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*FullOutputPath))
    {
        PlatformFile.CreateDirectoryTree(*FullOutputPath);
        UE_LOG(LogTemp, Warning, TEXT("Created download directory: %s"), *FullOutputPath);
    }
    
    // 1. Read the JSON file content into a string
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *JsonFilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file at: %s"), *JsonFilePath);
        return;
    }

    // 2. Prepare for JSON Deserialization (parsing)
    TArray<TSharedPtr<FJsonValue>> JsonArray;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonArray))
    {
        UE_LOG(LogTemp, Warning, TEXT("Successfully parsed JSON array with %d elements."), JsonArray.Num());
        
        // 3. Loop through the array and start downloads
        for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
        {
            TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
            if (JsonObject.IsValid())
            {
                // Extract required fields
                FString ItemName = JsonObject->GetStringField(TEXT("name"));
                
                // Sanitization is important for file paths!
                FString CleanName = SanitizeFilename(ItemName);

                // --- MODIFIED: Extract Shop Images (shop_image and shop_image_webp) ---
                FString ShopImageUrl = JsonObject->GetStringField(TEXT("shop_image"));
                FString ShopWebpUrl = JsonObject->GetStringField(TEXT("shop_image_webp"));

                // Download standard shop image
                if (!ShopImageUrl.IsEmpty())
                {
                    // Use a unique name for the shop image: ItemName_shop
                    FString FinalName = CleanName + TEXT("_shop"); 
                    FItemDownloadData PngData = { FinalName, ShopImageUrl, GetFileExtensionFromUrl(ShopImageUrl), FullOutputPath };
                    HandleItemDownload(PngData);
                }

                // Download webp shop image
                if (!ShopWebpUrl.IsEmpty())
                {
                    // Use a unique name for the webp shop image: ItemName_shop_webp
                    FString FinalName = CleanName + TEXT("_shop_webp");
                    FItemDownloadData WebpData = { FinalName, ShopWebpUrl, GetFileExtensionFromUrl(ShopWebpUrl), FullOutputPath };
                    HandleItemDownload(WebpData);
                }
                // ---------------------------------------------------------------------
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON string. Check structure (should be a root array)."));
    }
}

void UImageDownloader::HandleItemDownload(FItemDownloadData DownloadData)
{
    if (DownloadData.ImageUrl.IsEmpty() || DownloadData.ItemName.IsEmpty())
    {
        return;
    }

    // 1. Create the HTTP Request
    FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb(TEXT("GET"));
    HttpRequest->SetURL(DownloadData.ImageUrl);

    // 2. Bind the delegate (callback) to handle the response
    // We capture the DownloadData by value to ensure it's available when the async request finishes
    // Note: BindStatic is used here as a simplification from your original code.
    // In a real project, consider using BindUObject to ensure object lifetime management if this were not a static function.
    HttpRequest->OnProcessRequestComplete().BindStatic(&UImageDownloader::OnDownloadComplete, DownloadData);

    // 3. Send the request
    if (!HttpRequest->ProcessRequest())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to send HTTP request for: %s"), *DownloadData.ImageUrl);
    }
}

void UImageDownloader::OnDownloadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FItemDownloadData DownloadData)
{
    // Check for network success and HTTP success (200-299)
    if (!bWasSuccessful || !Response.IsValid() || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        UE_LOG(LogTemp, Error, TEXT("Download failed for %s. Code: %d"), *DownloadData.ImageUrl, Response.IsValid() ? Response->GetResponseCode() : 0);
        return;
    }

    // 1. Get the binary data (Array of Bytes)
    const TArray<uint8>& BinaryData = Response->GetContent();

    // 2. Construct the final file path
    FString FileName = DownloadData.ItemName + DownloadData.ImageExtension;
    FString FilePath = FPaths::Combine(DownloadData.OutputDirectory, FileName);

    // 3. Save the binary array to the file
    if (FFileHelper::SaveArrayToFile(BinaryData, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ Successfully saved: %s"), *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Failed to save file to: %s"), *FilePath);
    }
}
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Net.Http;
using System.Net;

namespace TestApp
{
    internal class CHttpClient
    {
        public async Task UploadFileAsync(string filePath, string url)
        {
            using (var client = new HttpClient())
            {
                try
                {
                    // Prepare the file content for upload
                    var fileContent = new MultipartFormDataContent();
                    var fileStream = new FileStream(filePath, FileMode.Open, FileAccess.Read);
                    var fileBytes = new byte[fileStream.Length];
                    await fileStream.ReadAsync(fileBytes, 0, (int)fileStream.Length);
                    var byteArrayContent = new ByteArrayContent(fileBytes);

                    // Add the file to the POST body with the key 'file' and the file name
                    byteArrayContent.Headers.ContentDisposition = new System.Net.Http.Headers.ContentDispositionHeaderValue("form-data")
                    {
                        Name = "\"file\"",
                        FileName = $"\"{Path.GetFileName(filePath)}\""
                    };
                    fileContent.Add(byteArrayContent);

                    // Send POST request to upload the file
                    var response = await client.PostAsync(url, fileContent);

                    if (response.IsSuccessStatusCode)
                    {
                        Console.WriteLine($"File uploaded successfully: {filePath}");
                    }
                    else
                    {
                        Console.WriteLine($"Failed to upload file: {response.ReasonPhrase}");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error uploading file: {ex.Message}");
                }
            }
        }
    }
}
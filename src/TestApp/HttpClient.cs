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
        private Action<string> logViewerAction;

        internal CHttpClient(Action<string> updateLogViewer)
        {
            logViewerAction = updateLogViewer;
        }
        
        private string ComputeSha256Hash(string filePath)
        {
            System.Security.Cryptography.SHA256 sha256 = System.Security.Cryptography.SHA256.Create();
            using (FileStream fileStream = File.OpenRead(filePath))
            {
                byte[] hash = sha256.ComputeHash(fileStream);
                return BitConverter.ToString(hash).Replace("-", "").ToLowerInvariant();
            }
        }

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
                    byteArrayContent.Headers.Add("File-Checksum", ComputeSha256Hash(filePath));

                    // Send POST request to upload the file
                    var response = await client.PostAsync(url, fileContent);

                    if (response.IsSuccessStatusCode)
                    {
                        if(response.StatusCode == HttpStatusCode.OK)
                            logViewerAction?.Invoke($"File uploaded successfully: {filePath}, size: {fileStream.Length}, reponse code: {response.StatusCode}");
                        else
                            logViewerAction?.Invoke($"File uploaded failed: {filePath}, size: {fileStream.Length}, reponse code: {response.StatusCode}, {response.ReasonPhrase}");
                    }
                    else
                    {
                        logViewerAction?.Invoke($"Failed to upload file: {response.StatusCode}, {response.ReasonPhrase}");
                    }
                }
                catch (Exception ex)
                {
                    logViewerAction?.Invoke($"Error uploading file: {ex.Message}");
                }
            }
        }
    }
}
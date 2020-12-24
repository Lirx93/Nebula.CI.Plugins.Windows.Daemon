using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;

namespace Nebula.CI.Plugins.WindowsService.Controllers
{
    [Route("[controller]")]
    [ApiController]
    public class CmdController : ControllerBase
    {
        public CmdController()
        {   
        }

        /// <summary>
        /// 执行命令
        /// </summary>
        [HttpPost("")]
        public async Task<ActionResult> ExecCmd([FromForm] IFormCollection formCollection)
        {
            string path = "C:\\nebula.ci\\" + DateTime.Now.ToFileTimeUtc().ToString(); 
            System.IO.Directory.CreateDirectory(path);
            
            foreach (var file in formCollection.Files) {
                var filePath = path + $"/{file.FileName}";
                using (var fs = new FileStream(filePath, FileMode.Create)) {
                    await file.OpenReadStream().CopyToAsync(fs);
                }
                var extension = Path.GetExtension(file.FileName);
                if (extension == ".zip") {
                    ZipFile.ExtractToDirectory(filePath, path);             
                }
            }

            string cmd = formCollection["CMD"];
            string resultPath = formCollection["ResultPath"];
            System.IO.Directory.CreateDirectory(path + "/" + resultPath);
            cmd = cmd.Replace("%2F", "/");
            string[] substr = cmd.Split(" ");
            string command = substr[0];
            string param = "";
            for(int i = 1;i<substr.Length;i++)
            {
                param += substr[i] + " ";
            }
            
            if(command != "contestbed" && command != "contbrun")
            {
                resultPath = path + "/" + resultPath;
            }
            else
            {
                resultPath = "C:\\LDRA_Workarea_CEE_9.4.6\\Tbwrkfls";
                try
                {
                    DirectoryInfo dir = new DirectoryInfo(resultPath);
                    FileSystemInfo[] fileinfo = dir.GetFileSystemInfos();  //返回目录中所有文件和子目录
                    foreach (FileSystemInfo i in fileinfo)
                    {
                        if (i is DirectoryInfo)            //判断是否文件夹
                        {
                            DirectoryInfo subdir = new DirectoryInfo(i.FullName);
                            subdir.Delete(true);          //删除子目录和文件
                        }
                        else
                        {
                            FileInfo file = new FileInfo(i.FullName);
                            file.Delete();      //删除指定文件
                        }
                    }               
                }
                catch (Exception e)
                {
                    throw;
                }
            }
            string ret = "command: " + command +"  param：" + param +" \n";
            ret += "workpath: " + path + "\n";
            //创建一个ProcessStartInfo对象 使用系统shell 指定命令和参数 设置标准输出
            var psi = new ProcessStartInfo(command, param) {RedirectStandardOutput = true};
            psi.WorkingDirectory = path;
            //启动
            Process proc = null;
            try
            {
                proc=Process.Start(psi);
            }
            catch(Exception e)
            {
                return NotFound("Cmd error : " + e.ToString());
            }
            
            if (proc == null)
            {
                return NotFound("Can not exec.");
            }
            else
            {
                ret += "-------------Start read standard output--------------\n";
                //开始读取
                using (var sr = proc.StandardOutput)
                {
                    while (!sr.EndOfStream)
                    {
                        ret += sr.ReadToEnd();
                    }

                    if (!proc.HasExited)
                    {
                        proc.Kill();
                    }
                }
                ret += "---------------Read end------------------\n";
                ret += $"Total execute time :{(proc.ExitTime-proc.StartTime).TotalMilliseconds} ms\n";
                ret += $"Exited Code ： {proc.ExitCode}\n";
                //ret += $"Error ： {proc.StandardError.ReadToEnd()}\n";

                await System.IO.File.WriteAllTextAsync(resultPath + "/log", ret);
                if(System.IO.File.Exists("C:\\nebula.ci\\result.zip"))
                {
                    System.IO.File.Delete("C:\\nebula.ci\\result.zip");
                }
                
                ZipFile.CreateFromDirectory(resultPath, "C:\\nebula.ci\\result.zip");
                //ZipFile.CreateFromDirectory("D:\\LDRA_Workarea_CEE_9.4.6\\Tbwrkfls", Directory.GetCurrentDirectory() + "/wwwroot/result.zip");
                var stream = System.IO.File.OpenRead("C:\\nebula.ci\\result.zip");
                return File(stream, "application/zip", "result.zip");
            }
        }

    }
}

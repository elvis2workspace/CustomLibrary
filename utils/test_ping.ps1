function test_ping($iplist)
{
    foreach ($myip in $iplist)
    {
        $strQuery = "select * from win32_pingstatus where address = '$myip'"
        # 利用 Get-WmiObject 送出 ping 的查詢
        $wmi = Get-WmiObject -query $strQuery
        if ($wmi.statuscode -eq 0)
        {
            return "Pinging`t$myip...`tsuccessful"
        }
        else
        {
            return "Pinging`t$myip...`tErrorCode:" + $wmi.statuscode
        }
    }
}

test_ping args[0]
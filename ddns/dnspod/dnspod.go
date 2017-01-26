package main

import (
	"crypto/tls"
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
	"os"
	"path/filepath"
	"regexp"
	"strconv"
	"strings"

	"github.com/gitwillsky/slimgo/config"
)

// DomainList域名列表
type DomainsList struct {
	RStatus  Status     `json:"status"`
	RInfo    DomainInfo `json:"info"`
	RDomains []Domains  `json:"domains"`
}

// Status 返回状态信息
type Status struct {
	Code      string `json:"code"`
	Message   string `json:"message"`
	CreatedAt string `json:"created_at"`
}

// DomainInfo 域名信息
type DomainInfo struct {
	TotalDomain int `json:"domain_total"`
	AllTotal    int `json:"all_total"`
}

// Domains 域名字段
type Domains struct {
	ID      int    `json:"id"`
	Status  string `json:"status"`
	GroupID string `json:"group_id"`
	TTL     string `json:"ttl"`
	Name    string `json:"name"`
	Records string `json:"records"`
}

// RecordList 记录列表
type RecordList struct {
	Records     []Record     `json:"records"`
	RStatus     Status       `json:"status"`
	RrecordInfo RecordInfo   `json:"info"`
	Domain      RecordDomain `json:"domain"`
}

// RecordInfo 记录信息
type RecordInfo struct {
	SubDomains  string `json:"sub_domains"`
	RecordTotal string `json:"record_total"`
}

// RecordDomain 记录域名信息
type RecordDomain struct {
	ID   int    `json:"id"`
	Name string `json:"name"`
}

// Record 记录字段
type Record struct {
	ID     string `json:"id"`
	Name   string `json:"name"`
	Type   string `json:"type"`
	TTL    string `json:"ttl"`
	Value  string `json:"value"`
	Status string `json:"status"`
	Line   string `json:"line"`
	Mx     string `json:"mx"`
}

// RecordUpdateList 更新的RecordList
type RecordUpdateList struct {
	DomainID   int
	RecordID   string
	RecordType string
	RecordLine string
	RecordMX   string
	SubDomain  string
	OldValue   string
}

// 用户相关信息
var (
	token string
)

// UpdateRecords 更新域名记录
func UpdateRecords(record RecordUpdateList) error {
	var ip string
	// GET ip Value
	resp, e := http.Get("http://ip.3322.org/")
	if e != nil {
		return e
	}
	defer resp.Body.Close()
	result, er := ioutil.ReadAll(resp.Body)
	if er != nil {
		return er
	}
	reg := regexp.MustCompile(`\d+\.\d+\.\d+\.\d+`)
	ip = reg.FindString(string(result))
	fmt.Println("New IP address: " + ip)
	dat := RecordList{}

	fmt.Printf("Update Record ID: %s  Domain ID : %d\n", record.RecordID, record.DomainID)
	resp, err := getClient().PostForm("https://dnsapi.cn/Record.Modify", url.Values{
		"login_token": {token},
		"format":      {"json"},
		"domain_id":   {strconv.Itoa(record.DomainID)},
		"record_id":   {record.RecordID},
		"record_type": {record.RecordType},
		"record_line": {record.RecordLine},
		"value":       {ip},
		"sub_domain":  {record.SubDomain},
		"mx":          {record.RecordMX},
	})
	if err != nil {
		return err
	}

	// 读取返回字节流并解析
	tempData, e := ioutil.ReadAll(resp.Body)
	if e != nil {
		return e
	}

	// 解析JSON
	if err = json.Unmarshal(tempData, &dat); err != nil {
		return err
	}

	fmt.Println(dat.RStatus.Message)
	return nil
}

// GetRecordList 获取记录列表
func GetRecordList(domainId string) (*RecordList, error) {
	var dat = &RecordList{}

	fmt.Println("Request domain record list...")
	resp, err := getClient().PostForm("https://dnsapi.cn/Record.List", url.Values{
		"login_token": {token},
		"format":      {"json"},
		"domain_id":   {domainId},
	})
	if err != nil {
		return nil, err
	}

	// 读取返回字节流并解析
	tempData, e := ioutil.ReadAll(resp.Body)
	if e != nil {
		return nil, e
	}

	// 解析JSON
	if err = json.Unmarshal(tempData, &dat); err != nil {
		return nil, err
	}

	//	fmt.Println(dat)

	return dat, nil
}

// GetDomainList 获取域名列表
func GetDomainList() (*DomainsList, error) {
	var dat = &DomainsList{}

	// 发送Post请求
	resp, err := getClient().PostForm("https://dnsapi.cn/Domain.List", url.Values{
		"login_token": {token},
		"format":      {"json"},
	})

	// 请求是否成功
	if err != nil {
		return nil, err
	}

	// 读取返回字节流并解析
	tempData, e := ioutil.ReadAll(resp.Body)
	if e != nil {
		return nil, e
	}
	// 解析JSON
	if err = json.Unmarshal(tempData, &dat); err != nil {
		return nil, err
	}
	//	fmt.Println(dat)

	// 判断用户是否登录成功
	if strings.Contains(dat.RStatus.Message, "fail") {
		return nil, errors.New("Login failed, Code: " + dat.RStatus.Code)
	}

	return dat, nil
}

// new https client
func getClient() *http.Client {
	tr := &http.Transport{
		TLSClientConfig:    &tls.Config{InsecureSkipVerify: true},
		DisableCompression: true,
	}

	client := &http.Client{Transport: tr}

	return client
}

func main() {
	var (
		domainList *DomainsList
		recordList *RecordList
		record     *Record
		err        error
		domainName string
		recordName string
		domainId   = -1
	)

	switch len(os.Args) {
	case 2:
		// use config
		if strings.TrimSpace(os.Args[1]) == "config" {
			// 打开配置文件
			Appconfig := config.New()
			dir, _ := filepath.Abs(filepath.Dir(os.Args[0]))
			if err = Appconfig.SetConfig("SlimGoConfig", dir+"/ddns.conf"); err != nil {
				panic("Open config file error: " + err.Error())
			}
			//  从配置文件读取信息
			token = Appconfig.GetString("token")
			domainName = Appconfig.GetString("domain")
			recordName = Appconfig.GetString("record")

			if domainName == "" {
				fmt.Println("Please check config domain value")
				return
			}

			if recordName == "" {
				fmt.Println("Please check config record value")
				return
			}

			if token == "" {
				fmt.Println("Please check config token value")
				return
			}

		} else {
			fmt.Println("Input params wrong")
			return
		}
	default:
		fmt.Println("Input params wrong")
		return
	}

	// 获取域名列表
	if domainList, err = GetDomainList(); err != nil {
		fmt.Println(err.Error())
		return
	}

	for _, v := range domainList.RDomains {
		if v.Name == domainName {
			domainId = v.ID
		}
	}

	if domainId == -1 {
		fmt.Println("domain not found !")
		return
	}

	// 获取选择域名的记录信息
	if recordList, err = GetRecordList(strconv.Itoa(domainId)); err != nil {
		fmt.Println(err.Error())
		return
	}

	for _, v := range recordList.Records {
		if v.Name == recordName {
			record = &v
			break
		}
	}

	fmt.Println(record)

	if record == nil {
		fmt.Println("record not found !")
		return
	}

	recordupdate := RecordUpdateList{
		DomainID:   domainId,
		RecordID:   record.ID,
		RecordType: record.Type,
		RecordLine: record.Line,
		RecordMX:   record.Mx,
		SubDomain:  record.Name,
		OldValue:   record.Value,
	}

	// update
	if err = UpdateRecords(recordupdate); err != nil {
		fmt.Println(err.Error())
	}
}

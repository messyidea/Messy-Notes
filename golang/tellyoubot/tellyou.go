// tellyou.go
package main

import (
	"encoding/json"
	"io"
	"log"
	"net/http"

	"github.com/tucnak/telebot"
)

var (
	Msgtoken   string = "message token"
	Bottoken   string = "your telegram bot token"
	Chatid     string = "your chatid with your bot"
	ListenAddr string = ":2333"
)

type TgBot struct {
	Bot      *telebot.Bot
	servemux *http.ServeMux
}

type SimpleDestination struct {
	ID string
}

func (self SimpleDestination) Destination() string {
	return self.ID
}

func (self *TgBot) SimpleSendMessage(chatid string, message string) error {
	return self.Bot.SendMessage(SimpleDestination{
		ID: chatid,
	}, message, nil)
}

type SingleMsg struct {
	Token   string
	Command string
	Data    string
}

func NewTgBot(secret string) (self *TgBot, err error) {
	self = new(TgBot)
	self.Bot, err = telebot.NewBot(secret)

	self.servemux = http.NewServeMux()
	self.servemux.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		var u SingleMsg
		if r.Body == nil {
			http.Error(w, "Please send a request body", 400)
			return
		}
		err := json.NewDecoder(r.Body).Decode(&u)
		if err != nil {
			http.Error(w, err.Error(), 400)
			return
		}

		log.Println("Get message: ", u)

		if u.Token != Msgtoken {
			http.Error(w, "Error token", 403)
			return
		}

		if u.Data == "" {
			io.WriteString(w, "Data can not be blank!")
			return
		}

		self.SimpleSendMessage(Chatid, u.Data)
		io.WriteString(w, "Done")
	})
	return
}

func (self *TgBot) ListenAndServe() error {
	return http.ListenAndServe(ListenAddr, self.servemux)
}

func main() {
	bot, err := NewTgBot(Bottoken)
	if err != nil {
		log.Fatalln(err)
	}

	err = bot.ListenAndServe()
	if err != nil {
		log.Fatalln(err)
	}
}

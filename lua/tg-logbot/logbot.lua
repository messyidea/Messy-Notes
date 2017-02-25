local print = print
local tconcat = table.concat
local tinsert = table.insert
local srep = string.rep
local type = type
local pairs = pairs
local tostring = tostring
local next = next
DATABASE_FILE = 'db.sqlite3'

function print_r(root)
  local cache = {  [root] = "." }
  local function _dump(t,space,name)
    local temp = {}
    for k,v in pairs(t) do
      local key = tostring(k)
      if cache[v] then
        tinsert(temp,"+" .. key .. " {" .. cache[v].."}")
                elseif type(v) == "table" then
                    local new_key = name .. "." .. key
                    cache[v] = new_key
                    tinsert(temp,"+" .. key .. _dump(v,space .. (next(t,k) and "|" or " " ).. srep(" ",#key),new_key))
                else
                    tinsert(temp,"+" .. key .. " [" .. tostring(v).."]")
                end
            end
            return tconcat(temp,"\n"..space)
        end
        print(_dump(root, "",""))
    end

function table_copy_table(ori_tab)
    if (type(ori_tab) ~= "table") then
        return nil
    end
    local new_tab = {}
    for i,v in pairs(ori_tab) do
        local vtyp = type(v)
        if (vtyp == "table") then
            new_tab[i] = table_copy_table(v)
        elseif (vtyp == "thread") then
            new_tab[i] = v
        elseif (vtyp == "userdata") then
            new_tab[i] = v
        else
            new_tab[i] = v
        end
    end
    return new_tab
end

function vardump(value, depth, key)
  local linePrefix = ""
  local spaces = ""
  
  if key ~= nil then
    linePrefix = "["..key.."] = "
  end
  
  if depth == nil then
    depth = 0
  else
    depth = depth + 1
    for i=1, depth do spaces = spaces .. "  " end
  end
  
  if type(value) == 'table' then
    mTable = getmetatable(value)
    if mTable == nil then
      print(spaces ..linePrefix.."(table) ")
    else
      print(spaces .."(metatable) ")
        value = mTable
    end   
    for tableKey, tableValue in pairs(value) do
      vardump(tableValue, depth, tableKey)
    end
  elseif type(value)  == 'function' or 
      type(value) == 'thread' or 
      type(value) == 'userdata' or
      value   == nil
  then
    print(spaces..tostring(value))
  else
    print(spaces..linePrefix.."("..type(value)..") "..tostring(value))
  end
end


local lgi = require ('lgi')
local notify = lgi.require('Notify')
notify.init ("Telegram updates")

chats = {}
sqlite3 = require("luasql.sqlite3")
      --db = sqlite3.open(DATABASE_FILE)
driver = sqlite3.sqlite3()
db = driver:connect(DATABASE_FILE)
res = db:execute[[
         CREATE TABLE if not exists messages (
            `id` INTEGER PRIMARY KEY,
            `from` TEXT,
            `to` TEXT,
            `text` TEXT,
            `date` INTEGER,
            `message_id` INTEGER,
            `type` TEXT,
            `from_id` INTEGER,
            `to_id` INTEGER
                               );
          ]]

function do_notify (user, msg)
  --local n = notify.Notification.new("message", "" .. user .. "".. msg .. "")
  local n = notify.Notification.new(user, msg)
  n:show ()
end

function dl_cb (arg, data)
end

function tdcli_update_callback (data)
  if (data.ID == "UpdateNewMessage") then
    local msg = data.message_
    local d = data.disable_notification_
    local chat = chats[msg.chat_id_]

    -- print_r(chat)
    -- print(type(chat))

    msg.chat_log = table_copy_table(chat)

    -- print_r(msg)
    -- print(msg.content_.ID)
    -- print("======================================")
    tdcli_function ({
      ID = "GetUser",
      user_id_ = data.message_.sender_user_id_
    }, user_callback, msg)
    -- print_r(msg)

    -- if msg.content_.ID == "MessageText" then
    --   tdcli_function ({
    --     ID = "GetUser",
    --     user_id_ = data.message_.sender_user_id_
    --   }, user_callback, msg)
    -- end
    if ((not d) and chat) then
      if msg.content_.ID == "MessageText" then
        do_notify (chat.title_, msg.content_.text_)
      else
        do_notify (chat.title_, msg.content_.ID)
      end
    end
    if msg.content_.ID == "MessageText" then
      if msg.content_.text_ == "!ping" then
        tdcli_function ({ID="SendMessage", chat_id_=msg.chat_id_, reply_to_message_id_=msg.id_, disable_notification_=0, from_background_=1, reply_markup_=nil, input_message_content_={ID="InputMessageText", text_="pong", disable_web_page_preview_=1, clear_draft_=0, entities_={}}}, dl_cb, nil)    
      elseif msg.content_.text_ == "!PING" then
        tdcli_function ({ID="SendMessage", chat_id_=msg.chat_id_, reply_to_message_id_=msg.id_, disable_notification_=0, from_background_=1, reply_markup_=nil, input_message_content_={ID="InputMessageText", text_="pong", disable_web_page_preview_=1, clear_draft_=0, entities_={[0]={ID="MessageEntityBold", offset_=0, length_=4}}}}, dl_cb, nil)    
      end
    end
  elseif (data.ID == "UpdateChat") then
    chat = data.chat_
    chats[chat.id_] = chat
  elseif (data.ID == "UpdateOption" and data.name_ == "my_id") then
    tdcli_function ({ID="GetChats", offset_order_="9223372036854775807", offset_chat_id_=0, limit_=20}, dl_cb, nil)    
  end
end

function user_callback(msg, message)
  -- print_r(msg)  --msg
  -- print("=================================")
  -- print_r(message) --user info
  log_message(msg, message)
end

function log_message(msg, usermsg)
  local fromName = 'DELETED_ACCOUNT'
  local fromId = 0
  local toName = ''
  local toId = 0
  local msgcontent = ''
  local msgid = ''
  if (usermsg.username_ ~= nil) then
    fromName = tostring(usermsg.username_)
  end
  if (usermsg.id_ ~= nil) then
    fromId = usermsg.id_
  end
  if (msg.chat_id_ ~= nil) then
    toId = msg.chat_id_
    -- toName = msg.chat_log.title_
  end
  if (msg.chat_log ~= nil) then
    if (msg.chat_log.title_ ~= nil) then
      toName = msg.chat_log.title_
    end
  end

  if (msg.content_.text_ ~= nil) then
    msgcontent = msg.content_.text_
  end
  if (msg.id_ ~= nil) then
    msgid = msg.id_
  end

  local sql = [[
                  INSERT INTO messages
                  (`from`, `from_id`, `to`, `to_id`, `text`, `date`, `message_id`, `type`)
                  VALUES (
                     ']] .. db:escape(fromName) .. [[',
                     ']] .. fromId .. [[',
                     ']] .. db:escape(toName) .. [[',
                     ']] .. toId .. [[',
                     ']] .. db:escape(msgcontent) .. [[',
                     ']] .. msg.date_ .. [[',
                     ']] .. msgid .. [[',
                     ']] .. msg.content_.ID .. [[')
                        ]]

  -- print(sql)
  res = db:execute(sql)
  -- print(res)
  print("messages saved: " .. tostring(res))
  -- print(msgid)
  -- print(fromName .. fromId .. toName .. toId .. msgcontent)
end
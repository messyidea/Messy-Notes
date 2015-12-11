flask
===

生成requirement.txt
```
pip freeze > requirements.txt
```


表单
```
from flask.ext.wtf import Form
from wtforms import StringFidld, SubmitField
from wtforms.validators import Required

PasswordField #密码字段

<form method="POST">
    {{ form.hdiien_tag() }}
    {{ form.name.lable }} {{ form.name }}
    {{ form.submit() }}
</form>
```

Flask-Login
---

使用的model必须实现下面的接口
is_authenticated()
is_active()
is_anonymous()
get_id()

可以通过current_user获取登陆用户信息

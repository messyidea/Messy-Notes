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


import i18n from 'i18next';
import { initReactI18next } from 'react-i18next';
import cn from './locales/zh-cn.json';
import en from './locales/en-us.json';

const resources = {
  cn: {
    translation: cn,
  },
  en: {
    translation: en,
  },
};
i18n.use(initReactI18next).init({
  resources,
  fallbackLng: 'cn',
});

export default i18n;

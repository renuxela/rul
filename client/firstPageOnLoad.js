async function firstPageOnLoad() {

    try {

        const response = fetch('/api/firstPageOnLoad');
        if (!response.ok) {
            throw new Error('Ошибка загрузки данных');
        }

        const jsonData = await response.json();

        document.getElementsByClassName('field__block-text')[0].textContent = jsonData.question_text;
        document.getElementsByClassName('field__block-text')[1].textContent = jsonData.rule_text;


    } catch(error) {
        console.error('Ошибка:', error);
    }

}

document.addEventListener('DOMContentLoaded', firstPageOnLoad);